const fs = require("fs");

function copyHashes(failHashFile, refHashFile) {
  try {
    const failHashData = JSON.parse(fs.readFileSync(failHashFile));
    const refHashData = JSON.parse(fs.readFileSync(refHashFile));

    for (const testFilename in failHashData.test) {
      if (!refHashData.test[testFilename]) {
        refHashData.test[testFilename] = {
          refs: [failHashData.test[testFilename].refs[0]],
        };
      } else if (refHashData.test[testFilename].refs[0] !== "")
        refHashData.test[testFilename].refs[0] =
          failHashData.test[testFilename].refs[0];
    }

    fs.writeFileSync(refHashFile, JSON.stringify(refHashData, null, 4));
  } catch (error) {
    // console.info(error);
  }
}

copyHashes(
  "test_report/results/test_cases/test_cases.json",
  "test_cases/test_cases.json"
);

copyHashes(
  "test_report/results/tests/style_tests/style_tests.json",
  "tests/style_tests.json"
);

copyHashes(
  "test_report/results/tests/features/features.json",
  "tests/features.json"
);

copyHashes(
  "test_report/results/tests/config_tests/config_tests.json",
  "tests/config_tests.json"
);

copyHashes(
  "test_report/results/tests/fixes/fixes.json",
  "tests/fixes.json"
);
