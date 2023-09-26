const div = document.createElement("div");
const chart = new Vizzu(div).initializing; // eslint-disable-line no-undef
const style = chart.then((chart) => {
  return chart.getComputedStyle();
});
style.then((style) => {
  const para = document.createElement("p");
  const node = document.createTextNode(JSON.stringify(style));
  para.appendChild(node);
  document.getElementById("style").appendChild(para);
});
