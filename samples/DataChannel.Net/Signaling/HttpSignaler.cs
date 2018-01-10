using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Threading.Tasks;

namespace DataChannel.Net.Signaling
{
    /// <summary>
    /// HttpSignaler instance is used to fire connection events.
    /// </summary>
    public class HttpSignaler : Signaler
    {
        private readonly HttpClient _httpClient = new HttpClient();
        private State _state;
        private Uri _baseHttpAddress;
        private int _myId;
        private string _clientName;
        public static ObservableCollection<Peer> _peers = new ObservableCollection<Peer>();

        /// <summary>
        /// Creates an instance of a HttpSignaler.
        /// </summary>
        /// <param name="server">Host name/IP.</param>
        /// <param name="port">Port to connect.</param>
        /// <param name="client_name">Client name.</param>
        public HttpSignaler(string server, int port, string client_name)
        {
            _state = State.NotConnected;
            _myId = -1;
            _clientName = client_name;
            _baseHttpAddress = new Uri("http://" + server + ":" + port);
        }

        /// <summary>
        /// Checks if connected to the server.
        /// </summary>
        /// <returns>True if connected to the server.</returns>
        public bool IsConnected()
        {
            return _myId != -1;
        }

        /// <summary>
        /// Connects to the server.
        /// </summary>
        public async Task<bool> Connect()
        {
            try
            {
                if (_state != State.NotConnected)
                {
                    OnServerConnectionFailure();
                    return true;
                }
                _state = State.SigningIn;
                await SendSignInRequestAsync();
                if (_state == State.Connected)
                {
                    // Start the long polling loop without await
                    var task = SendWaitRequestAsync();
                    return true;
                }
                else
                {
                    _state = State.NotConnected;
                    OnServerConnectionFailure();
                    return false;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("[Error] Signaling: Failed to connect to server: " + ex.Message);
                return false;
            }
        }

        /// <summary>
        /// Sends "sign_in" request to the server and waits for response.
        /// </summary>
        /// <returns>False if there is a failure, otherwise returns true.</returns>
        private async Task<bool> SendSignInRequestAsync()
        {
            try
            {
                string request = string.Format("sign_in?" + _clientName);

                // Send the request, await response
                HttpResponseMessage response = await _httpClient.GetAsync(_baseHttpAddress + request);
                HttpStatusCode status_code = response.StatusCode;

                string result = await response.Content.ReadAsStringAsync();
                if (result == null)
                    return false;
                int content_length = result.Length;

                string peer_name;
                int peer_id, peer_connected;
                if (!ParseServerResponse(result, status_code,
                    out peer_name, out peer_id, out peer_connected))
                    return false;

                if (_myId == -1)
                {
                    Debug.Assert(_state == State.SigningIn);
                    _myId = peer_id;
                    Debug.Assert(_myId != -1);

                    if (content_length > 0)
                    {
                        if (!ParseServerResponseAddPeersToList(result, status_code))
                            return false;
                        OnSignedIn();
                    }
                }
                else if (_state == State.SigningOut)
                {
                    Close();
                    OnDisconnected();
                }
                else if (_state == State.SigningOutWaiting)
                {
                    await SignOut();
                }
                if (_state == State.SigningIn)
                {
                    _state = State.Connected;
                }
                return true;
            }
            catch (Exception ex)
            {
                Debug.WriteLine("[Error] Signaling SendSignInRequestAsync: Failed to connect to server: " + ex.Message);
                await SignOut();
                return false;
            }
        }

        ///// <summary>
        ///// Long lasting loop to get notified about connected/disconnected peers.
        ///// </summary>
        private async Task SendWaitRequestAsync()
        {
            while (_state != State.NotConnected)
            {
                try
                {
                    string request = string.Format("wait?peer_id=" + _myId);

                    // Send the request, await response
                    HttpResponseMessage response =
                        await _httpClient.GetAsync(_baseHttpAddress + request,
                        HttpCompletionOption.ResponseContentRead);
                    HttpResponseHeaders header = response.Headers;
                    HttpStatusCode status_code = response.StatusCode;
                    if (response.StatusCode == HttpStatusCode.InternalServerError)
                    {
                        Debug.WriteLine("Internal server error, StatusCode: 500");
                        return;
                    }

                    int pragma = ParseHeaderGetPragma(header);

                    string result;
                    if (response.IsSuccessStatusCode)
                    {
                        result = await response.Content.ReadAsStringAsync();
                        if (_myId == pragma)
                        {
                            string peer_name;
                            int peer_id, peer_connected;
                            if (!ParseServerResponse(result, status_code,
                                out peer_name, out peer_id, out peer_connected))
                                continue;

                            // Add connected peer to the list, remove disconnected peer from the list
                            if (peer_connected == 1)
                                _peers.Add(new Peer(peer_id, peer_name));
                            else if (peer_connected == 0)
                                _peers.Remove(p => p.Id == peer_id);

                            if (peer_connected == 1)
                                OnPeerConnected(new Peer(peer_id, peer_name));
                            else if (peer_connected == 0)
                                OnPeerDisconnected(new Peer(peer_id, peer_name));
                        }
                        else
                        {
                            if (response.ToString().Contains("BYE"))
                                OnPeerHangup(new Peer(pragma, string.Empty));
                            else
                            {
                                Debug.WriteLine("OnMessageFromPeer! peer_id: " + pragma + " , result: " + result);
                                OnMessageFromPeer(new Peer(pragma, string.Empty, result));
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine("[Error] Signaling SendWaitRequestAsync, Message: " + ex.Message);
                }
                await Task.Delay(20000);
            }
        }

        private int ParseHeaderGetPragma(HttpResponseHeaders header)
        {
            string strHeader = header.ToString();
            string[] separatingChars = { "Pragma: " };
            string[] words = strHeader.Split(separatingChars, StringSplitOptions.RemoveEmptyEntries);

            string pr = words[1];
            int separator = pr.IndexOf("\n");
            string strPragma = pr.Substring(0, separator + 1);

            return strPragma.ParseLeadingInt();
        }

        private bool ParseServerResponseAddPeersToList(string content, HttpStatusCode status_code)
        {
            try
            {
                if (status_code != HttpStatusCode.OK)
                {
                    if (status_code == HttpStatusCode.InternalServerError)
                    {
                        Debug.WriteLine("[Error] Signaling ParseServerResponseSignIn: " + status_code);
                        OnPeerDisconnected(new Peer(0, string.Empty));
                        return false;
                    }
                    Close();
                    _myId = -1;
                    return false;
                }
                string[] separatingCharacter = { "\n" };
                string[] stringPeer = content.Split(separatingCharacter, StringSplitOptions.RemoveEmptyEntries);

                foreach (string s in stringPeer)
                {
                    string[] separatingChars = { "," };
                    string[] words = s.Split(separatingChars, StringSplitOptions.RemoveEmptyEntries);

                    string peer_name = words[0];
                    int peer_id = words[1].ParseLeadingInt();

                    _peers.Add(new Peer(peer_id, peer_name));
                    OnPeerConnected(new Peer(peer_id, peer_name));
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("[Error] Failed to parse server response (ex=" + ex.Message +
                    ")! Content(" + content.Length + ")=<" + content + ">");
                return false;
            }
            return true;
        }

        private bool ParseServerResponse(string content, HttpStatusCode status_code,
            out string peer_name, out int peer_id, out int peer_connected)
        {
            peer_name = "";
            peer_id = -1;
            peer_connected = 0;
            try
            {
                if (status_code != HttpStatusCode.OK)
                {
                    if (status_code == HttpStatusCode.InternalServerError)
                    {
                        Debug.WriteLine("[Error] Signaling ParseServerResponse: " + status_code);
                        OnPeerDisconnected(new Peer(0, string.Empty));
                        return false;
                    }
                    Close();
                    _myId = -1;
                    return false;
                }

                string[] separatingChars = { "," };
                string[] words = content.Split(separatingChars, StringSplitOptions.RemoveEmptyEntries);

                peer_name = words[0];
                peer_id = words[1].ParseLeadingInt();
                peer_connected = words[2].ParseLeadingInt();

                return true;
            }
            catch (Exception ex)
            {
                Debug.WriteLine("[Error] Failed to parse server response (ex=" + ex.Message +
                    ")! Content(" + content.Length + ")=<" + content + ">");
                return false;
            }
        }

        public override async Task<bool> SendToPeer(int peer_id, string message)
        {
            try
            {
                if (_state != State.Connected)
                    return false;

                Debug.Assert(IsConnected());

                if (!IsConnected() || peer_id == -1)
                    return false;

                string request =
                    string.Format(
                        "message?peer_id={0}&to={1} HTTP/1.0\r\n" +
                        "Content-Length: {2}\r\n" +
                        "Content-Type: text/plain\r\n" +
                        "\r\n" +
                        "{3}",
                        _myId, peer_id, message.Length, message);

                var content = new FormUrlEncodedContent(new[]
                {
                        new KeyValuePair<string, string>("message", message)
                });

                // Send request, await response
                HttpResponseMessage response = await _httpClient.PostAsync(
                    _baseHttpAddress + request, content);

                if (response.StatusCode != HttpStatusCode.OK)
                    return false;
            }
            catch (Exception ex)
            {
                Debug.WriteLine("[Error] Signaling SendToPeer: " + ex.Message);
            }
            return true;
        }

        /// <summary>
        /// Disconnects the user from the server.
        /// </summary>
        /// <returns>True if the user is disconnected from the server.</returns>
        public async Task<bool> SignOut()
        {
            if (_state == State.NotConnected || _state == State.SigningOut)
                return true;
            _state = State.SigningOut;

            if (_myId != -1)
            {
                string request = string.Format("sign_out?peer_id={0}", _myId);

                // Send request, await response
                HttpResponseMessage response = await _httpClient.GetAsync(
                    _baseHttpAddress + request);
            }
            else
                // Can occur if the app is closed before we finish connecting
                return true;

            _peers.Clear();
            _myId = -1;
            _state = State.NotConnected;
            return true;
        }

        public void Close()
        {
            _peers.Clear();
            _state = State.NotConnected;
        }
    }
}
