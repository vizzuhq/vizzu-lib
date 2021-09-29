export default class JsFiddle {

    static post(html, css, js){
        let form = document.createElement("form");
        form.method = "POST";
        form.action = "https://jsfiddle.net/api/post/library/pure/";
        form.target = "_blank";
        let params = {
            html: html,
            css: css,
            js: js
        }
        for (const key in params) {
            if (params.hasOwnProperty(key)) {
                const hiddenField = document.createElement('input');
                hiddenField.type = 'hidden';
                hiddenField.name = key;
                hiddenField.value = params[key];
                form.appendChild(hiddenField);
            }
        }
        document.body.appendChild(form);
        form.submit();
    }

}
