import { colors } from "/test/integration/test_options/style/colors.js";


export default class Rgba {
    static color(url) {
        let nameParts = url.pathname.split("/").pop().split(".");
        nameParts.pop();
        let name = nameParts.slice().join(".");
        let color = name.split("-")[0];
        let tr = name.split("-")[1];
        let type = name.split("-")[2];
        let ans = "rgba(" + colors[color].rgb + "," + parseFloat(tr) + ")";
        if (type === "w_spaces") {
            ans = ans.replaceAll(",", ", ");
        }
        return ans;
    }
}
