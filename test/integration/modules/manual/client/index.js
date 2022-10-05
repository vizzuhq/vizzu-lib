import ImgDiff from './imgdiff.js';

let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let urlTestFile = urlParams.get('testFile');
let urlTestIndex = urlParams.get('testIndex');
let urlVizzuUrl = urlParams.get('vizzuUrl');

let vizzuUrl = document.getElementById('vizzuUrl');
let vizzuRefUrl = 'https://vizzu-lib-main.storage.googleapis.com/lib';
let testCase = document.getElementById('testCase');
let frame = document.getElementById('frame');
let frameRef =document.getElementById('frame-ref');
let difCanvas = document.getElementById('canvas-dif');
let replay = document.getElementById('replay');

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
    let testCaseObject = JSON.parse(testCase.value);
	frame.src = `frame.html?testFile=${testCaseObject.testFile}&testType=${testCaseObject.testType}&testIndex=${testCaseObject.testIndex}&vizzuUrl=${vizzuUrl.value}`;
	if (vizzuUrl.value !== vizzuRefUrl)
	{
		difCanvas.style.display = "inline";
		frameRef.style.display = "inline";
		frameRef.src = `frame.html?testFile=${testCaseObject.testFile}&testType=${testCaseObject.testType}&testIndex=${testCaseObject.testIndex}&vizzuUrl=${vizzuRefUrl}`;
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
	replay.addEventListener('click', update);
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
        if (urlVizzuUrl) {
            lastSelected = data[urlVizzuUrl];
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
        let lastSelected = ''; 
		for (let i = 0; i < data.length; i++)
		{
			let actcase = JSON.stringify(data[i]);
            if (data[i].testFile === urlTestFile && data[i].testIndex == urlTestIndex) {
                lastSelected = actcase;
            }
            let actcaseName = data[i].testName;
			let selected = i == 0 ? 'selected="selected"' : '';
			testCase.innerHTML += `<option ${selected} value='${actcase}'>${actcaseName}</option>`;
		}
		if (lastSelected === '') lastSelected = JSON.stringify(data[0]);
		testCase.value = lastSelected;

		setupSelects();
		update();
	});
}

populateLibs();
