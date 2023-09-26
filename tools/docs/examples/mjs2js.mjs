const inputFileName = process.argv[2];
const assetsPath = process.argv[3];
const dataFileName = process.argv[4];
const dataName = process.argv[5];

import(inputFileName).then((module) => {
  const test = `
const dataLoaded = import("${assetsPath}/assets/data/${dataFileName}.js");
const mdChartLoaded = import("${assetsPath}/assets/javascripts/mdchart.js");
  
Promise.all([dataLoaded, mdChartLoaded]).then((results) => {
  const ${dataName} = results[0].${dataName};
  const MdChart = results[1].default;
  const mdchart = new MdChart(${dataName}, "./vizzu.js", "example");

  mdchart.create([
    {
      anims: [
        ${module.default}
      ]
    }
  ]);
});
`;
  console.log(test);
});
