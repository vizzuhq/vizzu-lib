document.addEventListener("DOMContentLoaded", (event) => {
  document.querySelectorAll("pre code").forEach((el) => {
    if (window.location.href.includes("LICENSE")) {
      return;
    }

    hljs.highlightElement(el); // eslint-disable-line no-undef
    if (
      window.location.href.includes("/examples/") ||
      window.location.href.includes("/showcases/")
    ) {
      hljs.lineNumbersBlock(el); // eslint-disable-line no-undef
    }
  });
});
