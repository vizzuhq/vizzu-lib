import { colors } from "/test/integration/test_options/style/colors.js";


export default class Rgb {
    static color(url) {
        let name = url.pathname.split("/").pop().split(".").shift();
        let color1 = "rgb(" + colors[name.split("-").shift()].rgb + ")";
        let color2 = "rgb(" + colors[name.split("-").pop()].rgb + ")";
        return {color1, color2};
    }
}
