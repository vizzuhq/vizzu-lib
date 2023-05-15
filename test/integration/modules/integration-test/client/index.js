function catchError(err) {
  console.error(err);
  let errMsg = err.toString();
  if (err.stack !== undefined) {
    errMsg = err.stack;
  }
  window.testData = { result: "ERROR", description: errMsg };
  document.title = "Finished";
}

function digestMessage(message) {
  return crypto.subtle.digest("SHA-256", message).then((hashBuffer) => {
    let hashArray = Array.from(new Uint8Array(hashBuffer));
    let hashHex = hashArray
      .map((b) => b.toString(16).padStart(2, "0"))
      .join("");
    return hashHex;
  });
}

function getAnimStep(testCasesModule, testType, testIndex) {
  let animStep;
  if (testType === "multi") {
    animStep = testCasesModule[testIndex]["animStep"];
  }
  if (!animStep) {
    animStep = 20;
  }
  return animStep;
}

function getTestSteps(testCasesModule, testType, testIndex) {
  let testSteps = [];
  if (testType === "single") {
    testSteps = testCasesModule;
  } else if (testType === "multi") {
    testSteps = testCasesModule[testIndex]["testSteps"];
  }
  return testSteps;
}

try {
  let queryString = window.location.search;
  let urlParams = new URLSearchParams(queryString);
  let testFile = urlParams.get("testFile");
  let testType = urlParams.get("testType");
  let testIndex = urlParams.get("testIndex");
  let vizzuUrl = urlParams.get("vizzuUrl");
  let refHash = urlParams.get("refHash");
  let createImages = urlParams.get("createImages");
  let testData = { result: "", hash: "", seeks: [], images: [], hashes: [] };

  import(vizzuUrl)
    .then((vizzuModule) => {
      var Vizzu = vizzuModule.default;
      return import(testFile + ".mjs").then((testCasesModule) => {
        let animStep = getAnimStep(
          testCasesModule.default,
          testType,
          testIndex
        );
        let seeks = [];
        for (
          let seek = parseFloat(animStep);
          seek <= 100;
          seek += parseFloat(animStep)
        ) {
          seeks.push(seek);
        }
        let chart = new Vizzu("vizzuCanvas");
        return chart.initializing.then((chart) => {
          let promise = Promise.resolve(chart);
          let promises = [];
          let testSteps = getTestSteps(
            testCasesModule.default,
            testType,
            testIndex
          );
          for (let i = 0; i < testSteps.length; i++) {
            console.log(i);
            promise = promise.then((chart) => {
              let animFinished = testSteps[i](chart);
              setTimeout(() => {
                let anim = chart.animation;
                anim.pause();
                testData.seeks[i] = [];
                testData.images[i] = [];
                testData.hashes[i] = [];
                seeks.forEach((seek) => {
                  seek = seek + "%";
                  testData.seeks[i].push(seek);
                  anim.seek(seek);
                  chart.render.updateFrame(true);
                  let canvasElement = document.getElementById("vizzuCanvas");
                  if (createImages !== "DISABLED") {
                    let dataURL = canvasElement.toDataURL();
                    testData.images[i].push(dataURL);
                  }
                  let ctx = canvasElement.getContext("2d");
                  let digestData = ctx.getImageData(
                    0,
                    0,
                    canvasElement.width,
                    canvasElement.height
                  );
                  let digest = digestMessage(digestData.data.buffer.slice());
                  digest = digest.then((digestBuffer) => {
                    testData.hashes[i].push(digestBuffer);
                  });
                  promises.push(digest);
                });
                anim.play();
              }, 10);
              return animFinished;
            });
          }
          return promise.then(() => {
            return Promise.all(promises).then(() => {
              testData.hashes.forEach((items) => {
                testData.hash += items.join("");
              });
              var buf = new ArrayBuffer(testData.hash.length * 2);
              var bufView = new Uint16Array(buf);
              for (var i = 0, strLen = testData.hash.length; i < strLen; i++) {
                bufView[i] = testData.hash.charCodeAt(i);
              }
              digestMessage(bufView).then((hash) => {
                hash = hash.substring(0, 7);
                testData.hash = hash;
                if (refHash !== "") {
                  if (refHash.includes(hash)) {
                    testData.result = "PASSED";
                    if (createImages === "FAILED") {
                      delete testData.images;
                    }
                  } else {
                    testData.description =
                      "hash: " + testData.hash + " " + "(ref: " + refHash + ")";
                    testData.result = "FAILED";
                  }
                } else {
                  testData.description =
                    "ref hash does not exist (hash: " + testData.hash + ")";
                  testData.result = "WARNING";
                }
                if (typeof window.testData === "undefined") {
                  window.testData = testData;
                  document.title = "Finished";
                }
              });
            });
          });
        });
      });
    })
    .catch((err) => {
      catchError(err);
    });
} catch (err) {
  catchError(err);
}
