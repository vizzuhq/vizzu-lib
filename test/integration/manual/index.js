let vizzuUrl = document.getElementById('vizzuUrl');
let testCase = document.getElementById('testCase');
let frame = document.getElementById('frame');

function update() {
	localStorage.setItem('vizzuUrl', vizzuUrl.value);
	localStorage.setItem('testCase', testCase.value);
	frame.src = `frame.html?testCase=${testCase.value}&vizzuUrl=${vizzuUrl.value}`;
}

function setupSelects()
{
	vizzuUrl.addEventListener("change", update);
	testCase.addEventListener("change", update);
}

function populateLibs()
{
	fetch('/getLibList')
	.then(response => response.json())
	.then(data => {
		for (let name in data)
		{
			let url = data[name];
			vizzuUrl.innerHTML += `<option value='${url}'>${name}</option>`;
		}
		let lastSelected = localStorage.getItem('vizzuUrl');
		if (lastSelected === '') lastSelected = data['localhost'];
		vizzuUrl.value = lastSelected;
		populateCases();
	});

}

function populateCases()
{
	fetch('/getTestList')
	.then(response => response.json())
	.then(data => {
		for (let i = 0; i < data.length; i++)
		{
			let actcase = data[i];
			let selected = i == 0 ? 'selected="selected"' : '';
			testCase.innerHTML += `<option ${selected} value='${actcase}'>${actcase}</option>`;
		}
		let lastSelected = localStorage.getItem('testCase');
		if (lastSelected === '') lastSelected = data[0];
		testCase.value = lastSelected;

		setupSelects();
		update();
	});
}

populateLibs();
