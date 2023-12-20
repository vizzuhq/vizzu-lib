class TestLib {
	constructor(url, name) {
		this.url = url
		this.name = name
	}

	createOption(select) {
		const option = document.createElement('option')
		option.value = this.url
		option.textContent = this.name
		select.appendChild(option)
	}
}

export default TestLib
