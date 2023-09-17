import { data } from "../../../../test_data/chart_types_eu.mjs";

const testSteps = [
  async (chart) => {
    let recordedChunks = [];
    let stream = chart.getCanvasElement().captureStream(30 /*fps*/);
    let mediaRecorder = new MediaRecorder(stream);

    mediaRecorder.ondataavailable = (e) => {
      recordedChunks.push(e.data);
    };

    mediaRecorder.onstop = (event) => {
      let blob = new Blob(recordedChunks, {
        type: "video/webm",
      });
      window.open(URL.createObjectURL(blob));
    };

    let anim = chart.animate({
      data: data,
      config: {
        x: "Year",
        y: ["Value 2 (+)", "Joy factors"],
        color: "Joy factors",
        title: "Video Export",
      },
    });

    anim.activated.then(() => {
      mediaRecorder.start();
    });

    anim.then((chart) => {
      mediaRecorder.stop();
    });

    return anim;
  },
];

export default testSteps;
