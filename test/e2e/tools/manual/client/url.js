export default class Url {
	constructor() {
		this.urlParams = Url.getQueryParams()
	}

	static getQueryParams() {
		const queryString = window.location.search
		return new URLSearchParams(queryString)
	}

	getQueryParam(param) {
		return this.urlParams.get(param)
	}
}
