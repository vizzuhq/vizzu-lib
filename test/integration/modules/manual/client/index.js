import ImgDiff from './imgdiff.js';

let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let urlTestCase = urlParams.get('testCase');
let urlVersion = urlParams.get('version');

let vizzuUrl = document.getElementById('vizzuUrl');
let vizzuRefUrl = 'https://vizzu-lib-main.storage.googleapis.com/lib';
let testCase = document.getElementById('testCase');
let frame = document.getElementById('frame');
let frameRef =document.getElementById('frame-ref');
let difCanvas = document.getElementById('canvas-dif');

function getDiff()
{
	let doc = frame.contentWindow.document;
	let docRef = frameRef.contentWindow.document;
	if (doc.vizzuImgData !== undefined && docRef.vizzuImgData !== undefined)
	{
		let w = doc.vizzuImgData.width;
		let h = doc.vizzuImgData.height;
		let res = ImgDiff.compare('move',
			doc.vizzuImgData.data, 
			docRef.vizzuImgData.data,
			w, h);

		let dif = new ImageData(res.diffData, w, h);
		difCanvas.width = 800;
		difCanvas.height = 500;
		const ctx = difCanvas.getContext('2d');
		ctx.clearRect(0, 0, w, h);
		ctx.putImageData(dif, 0, 0);
		doc.vizzuImgData = undefined;
		docRef.vizzuImgData = undefined;
		difCanvas.style=`border:1px solid ${res.match ? 'green' : 'red'}`;
	}
	else {
		setTimeout(getDiff, 200);
	}
}

function update() {
	localStorage.setItem('vizzuUrl', vizzuUrl.value);
	localStorage.setItem('testCase', testCase.value);
	frame.src = `frame.html?testCase=${testCase.value}&vizzuUrl=${vizzuUrl.value}`;
	if (vizzuUrl.value !== vizzuRefUrl)
	{
		difCanvas.style.display = "inline";
		frameRef.style.display = "inline";
		frameRef.src = `frame.html?testCase=${testCase.value}&vizzuUrl=${vizzuRefUrl}`;
		getDiff();
	}
	else {
		difCanvas.style.display = "none";
		frameRef.style.display = "none";
	}
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
        if (urlVersion) {
            lastSelected = data[urlVersion];
        }
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
		let lastSelected = urlTestCase 
			? urlTestCase : localStorage.getItem('testCase');
		if (lastSelected === '') lastSelected = data[0];
		testCase.value = lastSelected;

		setupSelects();
		update();
	});
}

populateLibs();
