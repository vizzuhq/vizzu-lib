const plausibleBasic = document.createElement("script");
plausibleBasic.setAttribute("defer", "");
plausibleBasic.setAttribute("data-domain", "lib.vizzuhq.com");
plausibleBasic.src = "https://plausible.io/js/script.outbound-links.js";

const plausibleOutboundLinkTracking = document.createElement("script");
plausibleOutboundLinkTracking.textContent =
  "window.plausible = window.plausible || function() { (window.plausible.q = window.plausible.q || []).push(arguments) }";

const documentHeadElement = document.getElementsByTagName("head")[0];
documentHeadElement.appendChild(plausibleBasic);
documentHeadElement.appendChild(plausibleOutboundLinkTracking);
