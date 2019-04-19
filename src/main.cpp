#include <DNSServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

// clang-format off
const String css =
  "/* Based off of https://github.com/sindresorhus/github-markdown-css */"
  ".markdown-body{-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%; line-height:1.5;color:#24292e;font-family:-apple-system, BlinkMacSystemFont,\"Segoe UI\",Helvetica,Arial,sans-serif,\"Apple Color Emoji\",\"Segoe UI Emoji\",\"Segoe UI Symbol\";font-size:16px;line-height:1.5;word-wrap:break-word}.markdown- body a{background-color:transparent}.markdown-body a:active,.markdown-body a:hover{outline-width:0}.markdown-body strong{font-weight:bolder}.markdown-body h1{font-size:2em;margin:0.67em 0}.markdown-body img{border-style:none}.markdown-body pre{font-family:monospace,monospace;font-size:1em}.markdown-body hr{box-sizing:content-box;height:0;overflow:visible}.markdown-body input{font:inherit;margin:0}.markdown-body input{overflow:visible}.markdown-body [type=\"checkbox\"]{box-sizing:border-box;padding:0}.markdown-body *{box-sizing:border-box}.markdown-body input{font-family:inherit;font-size:inherit;line-height:inherit}.markdown- body a{color:#0366d6;text-decoration:none}.markdown-body a:hover{text-decoration:underline}.markdown-body strong{font-weight:600}.markdown-body hr{height:0;margin:15px 0;overflow:hidden;background:transparent;border:0;border-bottom:1px solid #dfe2e5}.markdown-body hr::before{display:table;content:\"\"}.markdown-body hr::after{display:table;clear:both;content:\"\"}.markdown-body table{border-spacing:0;border-collapse:collapse}.markdown-body td,.markdown-body th{padding:0}.markdown-body h1,.markdown-body h2,.markdown-body h3,.markdown-body h4,.markdown-body h5,.markdown-body h6{margin-top:0;margin-bottom:0}.markdown-body h1{font-size:32px;font-weight:600}.markdown-body h2{font-size:24px;font-weight:600}.markdown-body h3{font-size:20px;font-weight:600}.markdown-body h4{font-size:16px;font-weight:600}.markdown-body h5{font-size:14px;font-weight:600}.markdown-body h6{font-size:12px;font-weight:600}.markdown-body p{margin-top:0;margin-bottom:10px}.markdown-body blockquote{margin:0}.markdown-body ul,.markdown-body ol{padding-left:0;margin-top:0;margin-bottom:0}.markdown-body ol ol,.markdown-body ul ol{list-style-type:lower-roman}.markdown-body ul ul ol,.markdown-body ul ol ol,.markdown-body ol ul ol,.markdown-body ol ol ol{list-style-type:lower-alpha}.markdown-body dd{margin-left:0}.markdown-body pre{margin-top:0;margin-bottom:0;font-family:\"SFMono-Regular\",Consolas, \"Liberation Mono\",Menlo,Courier,monospace;font-size:12px}.markdown-body::before{ display:table;content:\"\"}.markdown-body::after{display:table;clear:both; content:\"\"}.markdown-body>*:first-child{margin-top:0!important}. markdown-body>*:last-child{margin-bottom:0!important}.markdown-body a:not([href]){color:inherit;text-decoration:none}.markdown-body p,.markdown-body blockquote,.markdown-body ul,.markdown-body ol,.markdown-body dl,.markdown-body table,.markdown-body pre{margin-top:0;margin-bottom:16px}.markdown-body hr{height:0.25em;padding:0;margin:24px 0;background-color:#e1e4e8;border:0}.markdown-body blockquote{padding:0 1em;color:#6a737d;border-left:0.25em solid #dfe2e5}.markdown-body blockquote>:first-child{margin-top:0}.markdown-body blockquote>:last-child{margin-bottom:0}.markdown-body h1,.markdown-body h2,.markdown-body h3,.markdown-body h4,.markdown-body h5,.markdown-body h6{margin-top:24px;margin-bottom:16px;font-weight:600;line-height:1.25}. markdown-body h1{padding-bottom:0.3em;font-size:2em;border-bottom:1px solid #eaecef}.markdown-body h2{padding-bottom:0.3em;font-size:1.5em;border-bottom:1px solid #eaecef}.markdown-body h3{font-size:1.25em}.markdown-body h4{font-size:1em}.markdown-body h5{font-size:0.875em}.markdown-body h6{font-size:0.85em;color:#6a737d}.markdown-body ul,.markdown-body ol{padding-left:2em}.markdown-body ul ul,.markdown-body ul ol,.markdown-body ol ol,.markdown-body ol ul{margin-top:0;margin-bottom:0}.markdown-body li{word-wrap:break-all}.markdown-body li>p{margin-top:16px}.markdown-body li+li{margin-top:0.25em}.markdown-body dl{padding:0}.markdown-body dl dt{padding:0;margin-top:16px;font-size:1em;font-style:italic;font-weight: 600}.markdown-body dl dd{padding:0 16px;margin-bottom:16px}.markdown-body table{display:block;width:100%;overflow:auto}.markdown-body table th{font-weight:600}.markdown-body table th,.markdown-body table td{padding:6px 13px;border:1px solid #dfe2e5}.markdown-body table tr{background-color:#fff;border-top:1px solid #c6cbd1}.markdown-body table tr:nth-child(2n){background-color:#f6f8fa}.markdown-body img{max-width:100%;box-sizing:content-box;background-color:#fff}.markdown- body img[align=right]{padding-left:20px}.markdown-body img[align=left]{padding-right:20px}.markdown-body pre{padding:16px;overflow:auto;font-size:85%;line-height:1.45;background- color:#f6f8fa;border-radius:3px;word-wrap:normal}.markdown-body:checked+. radio-label{position:relative;z-index:1;border-color:#0366d6}.markdown- body hr{border-bottom-color:#eee}#log .v{color:#888}#log .d{color:#0DD}#log .c{color:magenta}#log .i{color:limegreen}#log .w{color:yellow}#log .e{color:red;font-weight:bold}#log{background-color:#1c1c1c}";
// clang-format on

// clang-format off
const String redirectHTML =
    ""
    "<!DOCTYPE html>"
    "<html>"
      "<head>"
        "<title>Redirect</title>"
        "<meta http-equiv='refresh' content='0; URL=http://192.168.1.1/ota'>"
      "</head>"
      "<body>"
        "<h1>Redirect...</h1>"
        "<p>"
          "Redirecting to OTA page at <a href='http://192.168.1.1/ota'>http://192.168.1.1/ota</a>"
        "</p>"
      "</body>"
    "</html>";
// clang-format on

// clang-format off
const String otaHTML =
    ""
    "<!DOCTYPE html>"
    "<html>"
      "<head>"
        "<title>OTA Update</title>"
        "<link rel='stylesheet' href='min.css'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1'>"
      "</head>"
      "<body class='markdown-body'>"
        "<h2>OTA Update</h2>"
        "<form method='POST' action='/update' enctype='multipart/form-data'>"
          "<input type='file' name='update'>"
          "<input type='submit' value='Update'>"
        "</form>"
      "</body>"
    "</html>";
// clang-format on

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
ESP8266HTTPUpdateServer httpUpdater;

const char* otaUpdatePath = "/ota";

void setup() {
  Serial.begin(115200);

  String mac = WiFi.macAddress();
  mac.replace(":", "");
  String macSuffix = mac.substring(mac.length() - 6);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("esp-ota-" + macSuffix);

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.sendHeader("Location", "http://192.168.1.1/ota", true);
    webServer.send(302, "text/html", redirectHTML);
  });
  webServer.on(otaUpdatePath,
               []() { webServer.send(200, "text/html", otaHTML); });
  webServer.on("/min.css", []() { webServer.send(200, "text/css", css); });
  //  httpUpdater.setup(&webServer);
  httpUpdater.setup(&webServer);
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
