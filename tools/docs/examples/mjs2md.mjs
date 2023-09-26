class PresetsMock {
  constructor() {
    return new Proxy(this, {
      get: (target, prop) => {
        return function (obj) {
          return `Vizzu.presets.${prop}(${JSON.stringify(obj, null, 2)})`;
        };
      },
    });
  }
}

class VizzuMock {
  constructor(title, description, data, assetsPath, dataFileName, dataName) {
    this.description = "";
    if (description) {
      this.description = description;
    }
    this.data = data;
    this.code = `---
data_url: ${assetsPath}/assets/data/${dataFileName}.js
---

# ${title}

<div id="example_01"></div>

??? info "Info - How to setup Vizzu"
    In \`HTML\`, create a placeholder element that will contain the rendered
    chart.

    \`\`\`html
    <html>
     <body>
      <div id="myVizzu">
      </div>
     </body>
    </html>

    \`\`\`

    In \`JavaScript\`, initialize and configure the chart:

    \`\`\`javascript
    import Vizzu from 'https://cdn.jsdelivr.net/npm/vizzu@latest/dist/vizzu.min.js'
    import { ${dataName} } from 'https://lib.vizzuhq.com/latest/assets/data/${dataFileName}.js'

    let chart = new Vizzu('myVizzu')

    chart.initializing

    chart.animate({
        ${dataName}
    })
    \`\`\`

${this.description}

\`\`\`javascript
`;

    this.end = `
\`\`\`

<script src="./main.js"></script>
`;
  }

  animate(chart, animOptions) {
    const params = [];
    const chartParams = [];

    if (chart.data && chart.data.filter) {
      if (JSON.stringify(chart.data) !== JSON.stringify(this.data)) {
        const fnCode = chart.data.filter.toString();
        chartParams.push(`data: {filter: ${fnCode}}`);
      }
    }
    if (chart.config) {
      if (
        typeof chart.config === "string" &&
        chart.config.startsWith("Vizzu.")
      ) {
        chartParams.push("config: " + chart.config);
      } else {
        chartParams.push("config: " + JSON.stringify(chart.config, null, 2));
      }
    }
    if (chart.style) {
      chartParams.push("style: " + JSON.stringify(chart.style, null, 2));
    }

    params.push(`{${chartParams.join(", ")}}`);

    if (animOptions) {
      if (typeof animOptions === "object") {
        params.push(`{${JSON.stringify(animOptions, null, 2)}}`);
      } else {
        params.push(`"${animOptions}"`);
      }
    }

    const args = params.join(", ");
    const fullCode = `chart.animate(${args});\n\n`;
    this.code += fullCode;
  }

  feature(name, enabled) {
    this.code += `chart.feature('${name}', ${enabled});\n\n`;
  }

  on(eventName, handler) {
    this.code += `chart.on('${eventName}', ${handler});\n\n`;
  }

  static get presets() {
    return new PresetsMock();
  }

  getCode() {
    return this.code + this.end;
  }
}

const inputFileName = process.argv[2];
const dataFilePath = process.argv[3];
const assetsPath = process.argv[4];
const dataFileName = process.argv[5];
const dataName = process.argv[6];
let title = process.argv[7];
const inputFileLoaded = import(inputFileName);
const dataFileLoaded = import(dataFilePath + "/" + dataFileName + ".mjs");
Promise.all([inputFileLoaded, dataFileLoaded]).then((results) => {
  const module = results[0];
  const description = module.description;
  if (module.title) {
    title = module.title;
  }
  const data = results[1][dataName];
  const chart = new VizzuMock(
    title,
    description,
    data,
    assetsPath,
    dataFileName,
    dataName
  );
  for (const testStep of module.default) {
    testStep(chart);
  }
  console.log(chart.getCode());
});
