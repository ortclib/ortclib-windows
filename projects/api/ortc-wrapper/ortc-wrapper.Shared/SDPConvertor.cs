using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
namespace ortc_wrapper.Shared
{
    class SDPConvertor
    {
        internal static void parseSDP(string sdp, out string sessionId, out string sessionVersion, out string streamId, out bool audioSupported, out bool videoSupported, out string ice_ufrag, out string ice_pwd, out string fingerprint, List<Dictionary<string, string>> extmaps, List<string> codecs)
        {
            bool valid = true;

            sessionId = null;
            sessionVersion = null;
            streamId = null;
            audioSupported = false;
            videoSupported = false;
            ice_ufrag = null;
            ice_pwd = null;

            fingerprint = null;

            extmaps = new List<Dictionary<string, string>>();
            codecs = new List<string>();

            string[] parts = sdp.Split("m=".ToCharArray());
            if (parts.Length < 2)
                return;

            valid = parseCommonSdp(parts[0], out sessionId, out sessionVersion, out streamId, out audioSupported, out videoSupported);

            for (int i = 1; i < parts.Length; i++)
            {
                parseMediaSdp(parts[i], out ice_ufrag, out ice_pwd, out fingerprint, extmaps, codecs);
            }
        }

        internal static bool parseCommonSdp(string sdp, out string sessionId, out string sessionVersion, out string streamId, out bool audioSupported, out bool videoSupported)
        {
            bool valid = true;
            sessionId = null;
            sessionVersion = null;
            streamId = null;
            audioSupported = false;
            videoSupported = false;

            try
            {
                TextReader reader = new StringReader(sdp);

                string line = reader.ReadLine();
                while (line != null)
                {
                    if ((line.Length == 0))
                    {
                        line = reader.ReadLine();
                        continue;
                    }
                    string value = line.Substring(2);
                    string[] parts = null;
                    int sep = -1;
                    try
                    {
                        switch (line[0])
                        {
                            case 'v':
                                int version = 0;
                                if (!int.TryParse(value, out version))
                                {
                                    throw new Exception(string.Format("Invalid Line {0}", line));
                                }
                                if (version != 0)
                                {
                                    throw new Exception(string.Format("Not supported version"));
                                }
                                break;
                            case 'o':
                                parts = value.Split(' ');
                                if (parts.Length != 6)
                                {
                                    throw new Exception(string.Format("Invalid Line {0}", line));
                                }
                                sessionId = parts[1];
                                sessionVersion = parts[2];
                                break;
                            case 's':
                                break;
                            case 't':
                                break;
                            case 'a':
                                sep = value.IndexOf(':');
                                string attrName;
                                string attrValue;

                                if (sep != -1)
                                    break;

                                attrName = value.Substring(0, sep);
                                attrValue = value.Substring(sep + 1);

                                if (attrName.Equals("group"))
                                {
                                    audioSupported = attrValue.Contains("audio");
                                    videoSupported = attrValue.Contains("video");
                                }
                                else if (attrName.Equals("msid-semantic"))
                                {
                                    streamId = attrValue;
                                }
                                break;
                        }
                    }
                    catch
                    {
                        throw new Exception(string.Format("Invalid Line {0}", line));
                    }
                    line = reader.ReadLine();
                }
            }
            catch
            {
                valid = false;
            }
            return valid;
        }

        internal static bool parseMediaSdp(string sdp, out string ice_ufrag, out string ice_pwd, out string fingerprint,List<Dictionary<string, string>> extmaps, List<string> codecs)
        {
            bool valid = true;
            ice_ufrag = null;
            ice_pwd = null;

            fingerprint = null;
            TextReader reader = new StringReader(sdp);

            string line = reader.ReadLine();
            while (line != null)
            {
                if ((line.Length == 0))
                {
                    line = reader.ReadLine();
                    continue;
                }
                string value = line.Substring(2);
                string[] parts = null;
                int sep = -1;
                try
                {
                    switch (line[0])
                    {
                        case 'c':
                            parts = value.Split(' ');
                            if (parts.Length != 3)
                            {
                                throw new Exception(string.Format("Invalid Line {0}", line));
                            }
                            string networkType = parts[0];
                            string addressType = parts[1];
                            parts = parts[2].Split('/');
                            if (parts.Length > 3)
                            {
                                throw new Exception(string.Format("Invalid Line {0}", line));
                            }
                            //TODO: Check if this is required
                            break;
                        case 'a':
                            sep = value.IndexOf(':');
                            string attrName;
                            string attrValue;

                            if (sep != -1)
                                break;

                            attrName = value.Substring(0, sep);
                            attrValue = value.Substring(sep + 1);

                            if (attrName.Equals("candidate"))
                            {
                                //org.ortc.RTCIceCandidate iceCandidate;
                                //iceCandidate = iceCandidateFromSdp(attrValue);
                            }
                            else if (attrName.Equals("rtcp"))
                            {

                            }
                            else if (attrName.Equals("ice-ufrag"))
                            {
                                ice_ufrag = attrValue;
                            }
                            else if (attrName.Equals("ice-pwd"))
                            {
                                ice_pwd = attrValue;
                            }
                            else if (attrName.Equals("fingerprint"))
                            {
                                fingerprint = attrValue;
                            }
                            else if (attrName.Equals("setup"))
                            {

                            }
                            else if (attrName.Equals("mid"))
                            {

                            }
                            else if (attrName.Equals("extmap"))
                            {
                                sep = value.IndexOf(' ');

                                if (sep != -1)
                                    break;

                                string extensionPreferredId = value.Substring(0, sep);
                                string extensionUri = value.Substring(sep + 1);
                                Dictionary<string, string> extension = new Dictionary<string, string>();
                                extension.Add(extensionPreferredId, extensionUri);
                                extmaps.Add(extension);
                            }
                            else if (attrName.Equals("sendrecv"))
                            {

                            }
                            else if (attrName.Equals("rtcp-mux"))
                            {

                            }
                            else if (attrName.Equals("rtpmap"))
                            {

                            }
                            else if (attrName.Equals("rtcp-fb"))
                            {

                            }
                            else if (attrName.Equals("fmtp"))
                            {

                            }
                            else if (attrName.Equals("maxptime"))
                            {

                            }
                            else if (attrName.Equals("ssrc"))
                            {

                            }
                            break;
                    }
                }
                catch
                {
                    throw new Exception(string.Format("Invalid Line {0}", line));
                }
                line = reader.ReadLine();
            }
            return valid;
        }
        /*  internal static void parseSDP(string sdp)
          {
              string sessionId = null;
              string sessionVersion = null;
              string streamId = null;

              bool audioSupported = false;
              bool videoSupported = false;

              string ice_ufrag = null;
              string ice_pwd = null;

              string fingerprint = null;
              List<Dictionary<string,string>> extmaps = new List<Dictionary<string, string>>();

              List<string> codecs = new List<string>();
              if (string.IsNullOrEmpty(sdp))
                  return;

              TextReader reader = new StringReader(sdp);

              string line = reader.ReadLine();
              while (line != null)
              {
                  if ((line.Length == 0))
                  {
                      line = reader.ReadLine();
                      continue;
                  }
                  string value = line.Substring(2);
                  string[] parts = null;
                  int sep = -1;
                  try
                  {
                      switch (line[0])
                      {
                          case 'v':
                              int version = 0;
                              if (!int.TryParse(value, out version))
                              {
                                  throw new Exception(string.Format("Invalid Line {0}", line));
                              }
                              if (version != 0)
                              {
                                  throw new Exception(string.Format("Not supported version"));
                              }
                              break;

                          case 'o':
                              parts = value.Split(' ');
                              if (parts.Length != 6)
                              {
                                  throw new Exception(string.Format("Invalid Line {0}", line));
                              }
                              sessionId = parts[1];
                              sessionVersion = parts[2];
                              break;
                          case 's':
                              break;
                          case 't':
                              break;

                          case 'c':
                              parts = value.Split(' ');
                              if (parts.Length != 3)
                              {
                                  throw new Exception(string.Format("Invalid Line {0}", line));
                              }
                              string networkType = parts[0];
                              string addressType = parts[1];
                              parts = parts[2].Split('/');
                              if (parts.Length > 3)
                              {
                                  throw new Exception(string.Format("Invalid Line {0}", line));
                              }
                              //TODO: Check if this is required
                              break;
                          case 'a':
                              sep = value.IndexOf(':');
                              string attrName;
                              string attrValue;

                              if (sep != -1)
                                  break;

                              attrName = value.Substring(0, sep);
                              attrValue = value.Substring(sep + 1);

                              if (attrName.Equals("candidate"))
                              {
                                  //org.ortc.RTCIceCandidate iceCandidate;
                                  //iceCandidate = iceCandidateFromSdp(attrValue);
                              }
                              else if (attrName.Equals("group"))
                              {
                                  audioSupported = attrValue.Contains("audio");
                                  videoSupported = attrValue.Contains("video");
                              }
                              else if (attrName.Equals("msid-semantic"))
                              {
                                  streamId = attrValue;
                              }
                              else if (attrName.Equals("rtcp"))
                              {

                              }
                              else if (attrName.Equals("ice-ufrag"))
                              {
                                  ice_ufrag = attrValue;
                              }
                              else if (attrName.Equals("ice-pwd"))
                              {
                                  ice_pwd = attrValue;
                              }
                              else if (attrName.Equals("fingerprint"))
                              {
                                  fingerprint = attrValue;
                              }
                              else if (attrName.Equals("setup"))
                              {

                              }
                              else if (attrName.Equals("mid"))
                              {

                              }
                              else if (attrName.Equals("extmap"))
                              {
                                  sep = value.IndexOf(' ');

                                  if (sep != -1)
                                      break;

                                  string extensionPreferredId = value.Substring(0, sep);
                                  string extensionUri = value.Substring(sep + 1);
                                  Dictionary<string, string> extension = new Dictionary<string, string>();
                                  extension.Add(extensionPreferredId, extensionUri);
                                  extmaps.Add(extension);
                              }
                              else if (attrName.Equals("sendrecv"))
                              {

                              }
                              else if (attrName.Equals("rtcp-mux"))
                              {

                              }
                              else if (attrName.Equals("rtpmap"))
                              {

                              }
                              else if (attrName.Equals("rtcp-fb"))
                              {

                              }
                              else if (attrName.Equals("fmtp"))
                              {

                              }
                              else if (attrName.Equals("maxptime"))
                              {

                              }
                              else if (attrName.Equals("ssrc"))
                              {

                              }

                              break;
                          case 'm':
                              parts = value.Split(' ');
                              if (parts.Length < 4)
                              {
                                  break;
                              }

                              string mediaType = parts[0];
                              string port = parts[1];
                              string protocol = parts[2];
                              // var formats = parts.Skip(3).ToList();

                              break;
                      }
                  }
                  catch
                  {
                      throw new Exception(string.Format("Invalid Line {0}", line));
                  }
                  line = reader.ReadLine();
              }
          }*/
    }
}
