import { colors } from "/test/integration/test_options/style/colors.js";


export default class Hex {
    static color(url) {
        let name = url.pathname.split("/").pop().split(".").shift();
        let color1 = "#" + colors[name.split("-").shift()].hex;
        let color2 = "#" + colors[name.split("-").pop()].hex;
        return {color1, color2};
    }
}
