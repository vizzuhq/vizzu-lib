import { colors } from "/test/integration/test_options/style/colors.js";


export default class Rgb {
    static color(url) {
        let name = url.pathname.split("/").pop().split(".").shift();
        let color = name.split("-").shift();
        let type = name.split("-").pop();
        let ans = "rgb(" + colors[color].rgb + ")";
        if (type === "w_spaces") {
            ans = ans.replaceAll(",", ", ");
        }
        return ans;
    }
}
