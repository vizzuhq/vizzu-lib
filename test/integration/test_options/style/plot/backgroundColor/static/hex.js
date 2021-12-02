import { colors } from "/test/integration/test_options/style/colors.js";


export default class Hex {
    static color(url) {
        let name = url.pathname.split("/").pop().split(".").shift();
        let color = name.split("-").shift();
        let type = name.split("-").pop();
        let ans = "#" + colors[color].hex.toUpperCase();
        if (type === "lowercase") {
            ans = ans.toLowerCase();
        }
        return ans;
    }
}
