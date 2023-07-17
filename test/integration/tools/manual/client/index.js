import TestLib from "./test-lib.js"
import { TestCase, TestCaseResult, TestCaseStatus } from "./test-case.js"
import ImgDiff from "./imgdiff.js";

class ManualClient {
  constructor() {
    this.urlParams = ManualClient.getUrlQueryParams();

    this.vizzuUrl = document.querySelector("#vizzuUrl");
    this.vizzuRef = document.querySelector("#vizzuRef");
    this.testCase = document.querySelector("#testCase");
    this.frame = document.querySelector("#frame");
    this.frameRef = document.querySelector("#frame-ref");
    this.difCanvas = document.querySelector("#canvas-dif");
    this.replay = document.querySelector("#replay");
    this.play = document.querySelector("#play");
    this.validate = document.querySelector("#validate");

    this.setupUserInterface();
    Promise.all([
      this.populateLibs(),
      this.populateCases(),
    ]).then(() => {
      this.update();
    });
  }

  static getUrlQueryParams() {
    const queryString = window.location.search;
    return new URLSearchParams(queryString);
  }

  setupUserInterface() {
    this.setupSelects()
    this.setupButtons();
  }

  populateLibs() {
    return fetch("/getLibs")
      .then((response) => response.json())
      .then((data) => {
        const libraries = Object.entries(data).map(([name, url]) => new TestLib(url, name));
        this.createLibOptions(this.vizzuUrl, libraries);
        this.createLibOptions(this.vizzuRef, libraries);

        const lastSelectedUrl = data[this.getUrlQueryParam("vizzuUrl")] || localStorage.getItem("vizzuUrl");
        this.vizzuUrl.value = lastSelectedUrl;
        if (!this.vizzuUrl.value) this.vizzuUrl.value = data["localhost"];

        const lastSelectedRefUrl = data[this.getUrlQueryParam("vizzuRefUrl")] || localStorage.getItem("vizzuRef");
        this.vizzuRef.value = lastSelectedRefUrl;
        if (!this.vizzuRef.value) this.vizzuRef.value = data["HEAD"] || ldata["localhost"];
      });
  }

  populateCases() {
    return fetch("/getTests")
      .then((response) => response.json())
      .then((data) => {
        let lastSelected = localStorage.getItem("testCase");
        const testCases = data.map(testData => new TestCase(
          testData.testConfig,
          testData.testFile,
          testData.testIndex,
          testData.testName,
          testData.testResult,
          testData.testSuite,
          testData.testType
        ));

        testCases.forEach((testCase, index) => {
          if (
            testCase.testFile === this.getUrlQueryParam("testFile") &&
            testCase.testIndex == this.getUrlQueryParam("testIndex")
          ) {
            lastSelected = JSON.stringify(testCase);
          }
          this.createTestCaseOption(this.testCase, testCase, index);
        });

        this.testCase.value = lastSelected;
        if (!this.testCase.value) this.testCase.value = JSON.stringify(testCases[0]);
      });
  }

  update() {
    localStorage.setItem("vizzuUrl", this.vizzuUrl.value);
    localStorage.setItem("vizzuRef", this.vizzuRef.value);
    localStorage.setItem("testCase", this.testCase.value);

    const testCaseObject = JSON.parse(this.testCase.value);
    this.frame.src = `frame.html?testFile=${testCaseObject.testFile}&testType=${testCaseObject.testType}&testIndex=${testCaseObject.testIndex}&vizzuUrl=${this.vizzuUrl.value}`;

    if (this.vizzuUrl.value !== this.vizzuRef.value) {
      this.difCanvas.style.display = "inline";
      this.frameRef.style.display = "inline";
      this.frameRef.src = `frame.html?testFile=${testCaseObject.testFile}&testType=${testCaseObject.testType}&testIndex=${testCaseObject.testIndex}&vizzuUrl=${this.vizzuRef.value}`;
      this.getDiff();
    } else {
      this.difCanvas.style.display = "none";
      this.frameRef.style.display = "none";
    }
    this.updateTestCaseOptions();
    this.connectSliders().then((charts) => {
      setTimeout(() => {
        this.run(charts);
      }, 0);
    });
  }

  setupSelects() {
    this.vizzuUrl.addEventListener("change", () => this.update());
    this.vizzuRef.addEventListener("change", () => this.update());
    this.testCase.addEventListener("change", () => this.update());
   }

  setupButtons() {
    this.replay.addEventListener("click", () => this.update());
    this.play.addEventListener("click", () => this.run([undefined, undefined]));
    this.validate.addEventListener("click", () => this.validateTestCase());
  }

  createLibOptions(select, libs) {
    libs.forEach(lib => {
      const option = document.createElement("option");
      option.value = lib.url;
      option.textContent = lib.name;
      select.appendChild(option);
    });
  }

  getUrlQueryParam(param) {
    return this.urlParams.get(param);
  }

  createTestCaseOption(select, testCase, index) {
    const option = document.createElement("option");
    option.value = JSON.stringify(testCase);
    option.selected = index === 0;
    option.setAttribute("name", testCase.testName);
    this.setTestCaseOptionResult(option, testCase);
    select.appendChild(option);
  }

  setTestCaseOptionResult(option, testCase, testResult) {
    if (testResult) testCase.testResult = testResult;
    option.setAttribute("result", testCase.testResult);
    option.setAttribute("background-color", TestCaseResult.getColor(testCase.testResult));
    option.textContent = testCase.testResult ? `${testCase.testName} | ${testCase.testResult}` : testCase.testName;
  }

  getDiff() {
    const doc = this.frame.contentWindow.document;
    const docRef = this.frameRef.contentWindow.document;

    if (doc.vizzuImgData && docRef.vizzuImgData && doc.vizzuImgIndex === docRef.vizzuImgIndex) {
      const { width: w, height: h, data } = doc.vizzuImgData;
      const res = ImgDiff.compare("move", data, docRef.vizzuImgData.data, w, h);

      const dif = new ImageData(res.diffData, w, h);
      this.difCanvas.width = 800;
      this.difCanvas.height = 500;
      const ctx = this.difCanvas.getContext("2d");
      ctx.clearRect(0, 0, w, h);
      ctx.putImageData(dif, 0, 0);
      doc.vizzuImgData = docRef.vizzuImgData = undefined;
      this.difCanvas.style.border = `1px solid ${res.match ? "green" : "red"}`;
    }

    setTimeout(() => this.getDiff(), 100);
  }

  updateTestCaseOptions() {
    this.testCase.querySelectorAll("option").forEach((option) => {
      option.style.backgroundColor = option.selected ? "rgba(206,206,206,255)" : option.getAttribute("background-color");
    });
  }

  connectSliders() {
    const waitForLoad = new Promise((resolve) => {
      this.frame.addEventListener("load", () => resolve());
    });

    const waitForLoadRef = new Promise((resolve) => {
      if (this.frameRef.style.display !== "none") {
        this.frameRef.addEventListener("load", () => resolve());
      } else {
        resolve();
      }
    });

    return Promise.all([waitForLoad, waitForLoadRef])
      .then(() => Promise.all([
        this.frame.contentWindow.setup,
        this.frameRef.contentWindow.setup
      ]))
      .then((setups) => {
        const slider = this.frame.contentWindow.document.getElementById("myRange");
        const sliderRef = this.frameRef.contentWindow.document.getElementById("myRange");
        slider.addEventListener("input", (e) => {
          this.frameRef.contentWindow.setSlider(e.target.value);
        });

        if (this.frameRef.style.display !== "none") {
          sliderRef.addEventListener("input", (e) => {
            this.frame.contentWindow.setSlider(e.target.value);
          });
        }

        return setups;
      });
  }

  run(charts) {
    this.frame.contentWindow.run(charts[0]);
    if (this.frameRef.style.display !== "none") {
      this.frameRef.contentWindow.run(charts[1]);
    }
  }

  validateTestCase() {
    const testCaseValue = this.testCase.value;
    fetch("/validateTestCase", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ testCaseValue }),
    })
      .then((response) => response.json())
      .then((data) => {
        if (data.status === TestCaseStatus.TYPES.UNCHANGED) {
          console.warn(`Hash ${data.status}`);
        } else if (data.status === TestCaseStatus.TYPES.ADDED || data.status === TestCaseStatus.TYPES.UPDATED) {
          console.log(`Hash ${data.status}`);
          const testCaseOption = this.testCase.options[this.testCase.selectedIndex];
          this.setTestCaseOptionResult(testCaseOption, JSON.parse(testCaseOption.value), TestCaseResult.TYPES.PASSED);
        } else {
          console.error("Hash validation failed:", data.status);
        }
      })
      .catch((error) => {
        console.error("Hash validation failed:", error);
      });
  }
}

const manualClient = new ManualClient();
