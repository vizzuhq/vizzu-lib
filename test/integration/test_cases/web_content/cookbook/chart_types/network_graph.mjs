function axisOptions(on) {
  return {
    axis: on,
    labels: on,
    markerGuides: false,
    interlacing: on,
    title: on ? "auto" : null,
  };
}

const testSteps = [
  async (chart) => {
    let Viz = (
      await import("https://cdn.jsdelivr.net/npm/@aduh95/viz.js@3.7.0/+esm")
    ).default;

    const locateFile = (fileName) =>
      "https://cdn.jsdelivr.net/npm/@aduh95/viz.js@3.7.0/dist/" + fileName;
    const onmessage = async function (event) {
      if (this.messageHandler === undefined) {
        const { default: init, onmessage } = await import(
          Module.locateFile("render.browser.js")
        );
        removeEventListener("message", onmessage);
        await init(Module);
        this.messageHandler = onmessage;
      }
      return this.messageHandler(event);
    };
    const vizOptions = {
      workerURL: URL.createObjectURL(
        new Blob(
          [
            "const Module = { locateFile:",
            locateFile.toString(),
            "};",
            "onmessage=",
            onmessage.toString(),
          ],
          { type: "application/javascript" },
        ),
      ),
    };

    let viz = new Viz(vizOptions);

    let dot = `
        graph G {
            run -- intr;
            intr -- runbl;
            runbl -- run;
            run -- kernel;
            kernel -- zombie;
            kernel -- sleep;
            kernel -- runmem;
            sleep -- swap;
            swap -- runswap;
            runswap -- new;
            runswap -- runmem;
            new -- runmem;
            sleep -- runmem;
        }
        `;

    let graph = await viz.renderJSONObject(dot, { engine: "neato" });

    let data = {
      series: [
        { name: "edge", type: "dimension" },
        { name: "part", type: "dimension" },
        { name: "node", type: "dimension" },
        { name: "x", type: "measure" },
        { name: "y", type: "measure" },
        { name: "data", type: "measure" },
      ],
      records: [],
    };

    for (let i = 0; i < graph.edges.length; i++) {
      let edge = graph.edges[i];
      let head = graph.objects[edge.head];
      let tail = graph.objects[edge.tail];
      let headPos = head.pos.split(",");
      let tailPos = tail.pos.split(",");
      let headName = head.name;
      let tailName = tail.name;
      data.records.push([i, "head", headName, headPos[1], headPos[0], i]);
      data.records.push([i, "tail", tailName, tailPos[1], tailPos[0], i]);
    }

    function rectCenter(rect) {
      return {
        x: rect.pos.x + rect.size.x / 2,
        y: rect.pos.y + rect.size.y / 2,
      };
    }

    let lastPos = null;
    chart.on("plot-marker-draw", (event) => {
      let pos = rectCenter(event.detail.rect);
      if (lastPos === null) {
        lastPos = pos;
      } else {
        let ctx = event.renderingContext;
        ctx.beginPath();
        let convert = chart.getConverter("plot-area", "relative", "canvas");
        let p0 = convert(lastPos);
        let p1 = convert(pos);
        ctx.moveTo(p0.x, p0.y);
        ctx.lineTo(p1.x, p1.y);
        ctx.stroke();
        lastPos = null;
      }
    });

    return chart.animate({
      data: data,
      config: {
        x: Object.assign({ set: "x" }, axisOptions(false)),
        y: Object.assign({ set: "y" }, axisOptions(false)),
        label: "node",
        noop: ["edge", "part"],
        geometry: "circle",
        title: "Network graph",
      },
    });
  },

  (chart) =>
    chart.animate({
      x: Object.assign({ set: "node" }, axisOptions(true)),
      y: Object.assign({ set: ["edge", "part", "data"] }, axisOptions(true)),
      label: null,
      noop: null,
      geometry: "rectangle",
    }),
];

export default testSteps;
