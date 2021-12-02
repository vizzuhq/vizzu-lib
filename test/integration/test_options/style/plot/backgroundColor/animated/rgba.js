import { colors } from "/test/integration/test_options/style/colors.js";


export default class Hexa {
    static color(url) {
        let nameParts = url.pathname.split("/").pop().split(".");
        nameParts.pop();
        let name = nameParts.slice().join(".");
        let color1 = name.split("-")[0];
        let tr1 = name.split("-")[1];
        let color2 = name.split("-")[2];
        let tr2 = name.split("-")[3];
        color1 = "rgba(" + colors[color1].rgb + "," + parseFloat(tr1) + ")";
        color2 = "rgba(" + colors[color2].rgb + "," + parseFloat(tr2) + ")";
        return {color1, color2};
    }
}
