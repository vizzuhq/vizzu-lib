import ImgDiff from "./imgdiff.js";

let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let urlTestFile = urlParams.get("testFile");
let urlTestIndex = urlParams.get("testIndex");
let urlVizzuUrl = urlParams.get("vizzuUrl");
let urlVizzuRefUrl = urlParams.get("vizzuRefUrl");

let vizzuUrl = document.querySelector("#vizzuUrl");
let vizzuRef = document.querySelector("#vizzuRef");
let testCase = document.querySelector("#testCase");
let frame = document.querySelector("#frame");
let frameRef = document.querySelector("#frame-ref");
let difCanvas = document.querySelector("#canvas-dif");
let replay = document.querySelector("#replay");
let play = document.querySelector("#play");

function getDiff() {
  let doc = frame.contentWindow.document;
  let docRef = frameRef.contentWindow.document;
  if (doc.vizzuImgData && docRef.vizzuImgData && doc.vizzuImgIndex === docRef.vizzuImgIndex) {
    let { width: w, height: h, data } = doc.vizzuImgData;
    let res = ImgDiff.compare("move", data, docRef.vizzuImgData.data, w, h);

    let dif = new ImageData(res.diffData, w, h);
    difCanvas.width = 800;
    difCanvas.height = 500;
    const ctx = difCanvas.getContext("2d");
    ctx.clearRect(0, 0, w, h);
    ctx.putImageData(dif, 0, 0);
    doc.vizzuImgData = docRef.vizzuImgData = undefined;
    difCanvas.style.border = `1px solid ${res.match ? "green" : "red"}`;
  }
  setTimeout(getDiff, 100);
}

function update() {
  localStorage.setItem("vizzuUrl", vizzuUrl.value);
  localStorage.setItem("vizzuRef", vizzuRef.value);
  localStorage.setItem("testCase", testCase.value);
  let testCaseObject = JSON.parse(testCase.value);
  frame.src = `frame.html?testFile=${testCaseObject.testFile}&testType=${testCaseObject.testType}&testIndex=${testCaseObject.testIndex}&vizzuUrl=${vizzuUrl.value}`;
  if (vizzuUrl.value !== vizzuRef.value) {
    difCanvas.style.display = "inline";
    frameRef.style.display = "inline";
    frameRef.src = `frame.html?testFile=${testCaseObject.testFile}&testType=${testCaseObject.testType}&testIndex=${testCaseObject.testIndex}&vizzuUrl=${vizzuRef.value}`;
    getDiff();
  } else {
    difCanvas.style.display = "none";
    frameRef.style.display = "none";
  }
  connectSliders().then((charts) => {
    setTimeout(() => {
      run(charts);
    }, 0);
  });

  testCase.querySelectorAll("option").forEach((option) => {
    option.style.backgroundColor = option.selected ? "rgba(206,206,206,255)" : option.getAttribute("background-color");
  });
}

function connectSliders() {
  let waitForLoad = new Promise((resolve) => {
    frame.addEventListener("load", () => {
      resolve();
    });
  });

  let waitForLoadRef = new Promise((resolve) => {
    if (frameRef.style.display !== "none") {
      frameRef.addEventListener("load", () => {
        resolve();
      });
    } else {
      resolve();
    }
  });

  return Promise.all([waitForLoad, waitForLoadRef])
    .then(() => {
      return Promise.all([
        frame.contentWindow.setup,
        frameRef.contentWindow.setup,
      ]);
    })
    .then((setups) => {
      let slider = frame.contentWindow.document.getElementById("myRange");
      let sliderRef = frameRef.contentWindow.document.getElementById("myRange");
      slider.addEventListener("input", (e) => {
        frameRef.contentWindow.setSlider(e.target.value);
      });
      if (frameRef.style.display !== "none") {
        sliderRef.addEventListener("input", (e) => {
          frame.contentWindow.setSlider(e.target.value);
        });
      }
      return setups;
    });
}

function run(charts) {
  frame.contentWindow.run(charts[0]);
  if (frameRef.style.display !== "none") {
    frameRef.contentWindow.run(charts[1]);
  }
}

function setupSelects() {
  vizzuUrl.addEventListener("change", update);
  vizzuRef.addEventListener("change", update);
  testCase.addEventListener("change", update);
  replay.addEventListener("click", update);
  play.addEventListener("click", () => run([undefined, undefined]));
}

function populateLibs() {
  fetch("/getLibList")
    .then((response) => response.json())
    .then((data) => {
      Object.entries(data).forEach(([name, url]) => {
        vizzuUrl.appendChild(getVizzuOption(url, name));
        vizzuRef.appendChild(getVizzuOption(url, name));
      });
      let lastSelected = data[urlVizzuUrl] || localStorage.getItem("vizzuUrl");
      vizzuUrl.value = lastSelected;
      if (!vizzuUrl.value) vizzuUrl.value = data["localhost"];
      let lastSelectedRef = data[urlVizzuRefUrl] || localStorage.getItem("vizzuRef");
      vizzuRef.value = lastSelectedRef;
      if (!vizzuRef.value) vizzuRef.value = data["HEAD"] || data["localhost"];
      populateCases();
    });
}

function getVizzuOption(url, name) {
  const option = document.createElement('option');
  option.value = url;
  const text = document.createTextNode(name);
  option.appendChild(text);
  return option;
}

function populateCases() {
  fetch("/getTestList")
    .then((response) => response.json())
    .then((data) => {
      let lastSelected = localStorage.getItem("testCase");
      for (let i = 0; i < data.length; i++) {
        let actcase = JSON.stringify(data[i]);
        if (
          data[i].testFile === urlTestFile &&
          data[i].testIndex == urlTestIndex
        ) {
          lastSelected = actcase;
        }
        let actcaseName = data[i].testName;
        let actcaseResult = data[i].testResult;
        let selected = i == 0 ? 'selected="selected"' : "";
        testCase.appendChild(getTestCaseOption(actcase, actcaseName, actcaseResult, selected));
      }
      testCase.value = lastSelected;
      if (!testCase.value) testCase.value = JSON.stringify(data[0]);

      setupSelects();
      update();
    });
}

function getTestCaseOption(testCase, testCaseName, testCaseResult, selected) {
  const option = document.createElement("option");
  option.value = testCase;
  option.selected = selected;
  option.textContent = testCaseResult ? `${testCaseName} | ${testCaseResult}` : testCaseName;
  option.setAttribute("background-color", getTestCaseBackgroundColorByResult(testCaseResult));
  return option;
}

function getTestCaseBackgroundColorByResult(testCaseResult) {
  if (testCaseResult === "PASS") {
    return "rgba(152,251,152,0.8)";
  } else if (testCaseResult === "FAIL") {
    return "rgba(255,153,153,0.8)";
  } else if (testCaseResult === "WARN") {
    return "rgba(255,255,153,0.8)";
  }
  return "";
}

populateLibs();
