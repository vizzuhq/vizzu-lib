import { colors } from "/test/integration/test_options/style/colors.js";
import { transparency } from "/test/integration/test_options/style/transparency.js";


export default class Hexa {
    static color(url) {
        let nameParts = url.pathname.split("/").pop().split(".");
        nameParts.pop();
        let name = nameParts.slice().join(".");
        let color = name.split("-")[0];
        let tr = name.split("-")[1];
        let type = name.split("-")[2];
        let ans = ("#" + colors[color].hex + transparency[tr]).toUpperCase();
        if (type === "lowercase") {
            ans = ans.toLowerCase();
        }
        return ans;
    }
}
