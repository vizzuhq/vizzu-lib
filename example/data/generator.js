function Rand(a) {
	return function() {
	  var t = a += 0x6D2B79F5;
	  t = Math.imul(t ^ t >>> 15, t | 1);
	  t ^= t + Math.imul(t ^ t >>> 7, t | 61);
	  return ((t ^ t >>> 14) >>> 0) / 4294967296;
	}
}

let rand = Rand(32);

function scale(x, min, max) {
	return min + x * (max-min);
}

function random(min, max) {
	return scale(rand(), min, max);
}

function addLinear(array, i0, x0, i1, x1)
{
	for (let i = i0; i <= i1; i++) {
		let f = (i - i0) / (i1 - i0);
		array[i] += x0 * (1-f) + x1 * f;
	}
}

function perlin()
{
	let res = new Array(8);
	res.fill(0);
	addLinear(res, 0, random(0,0.55), 7, random(0,0.55));
	addLinear(res, 0, random(0,0.3), 3, random(0,0.3));
	addLinear(res, 4, random(0,0.3), 7, random(0,0.3));
	for (let i = 0; i < 8; i++) res[i] += random(0,0.15);
	return res;
}

const countries = [
	['Europe',        'Hungary',          'C00'], 
	['Europe',        'Germany',          'C01'],
	['Europe',        'France',           'C02'],
	['Europe',        'Sweeden',          'C03'],
	['Europe',        'Finland',          'C04'],
	['Europe',        'Denmark',          'C05'],
	['Europe',        'Spain',            'C06'],
	['Europe',        'Portugal',         'C07'],
	['Europe',        'Netherland',       'C08'],
	['Europe',        'Belgium',          'C09'],
	['Europe',        'Slovakia',         'C10'],
	['Europe',        'Croatia',          'C11'],
	['Europe',        'Poland',           'C12'],
	['Europe',        'Checzia',          'C13'],
	['Europe',        'Switzerland',      'C14'],
	['Europe',        'Norway',           'C15'],
	['Europe',        'United Kingdom',   'C16'],
	['Europe',        'Ireland',          'C17'],
	['Europe',        'Italy',            'C18'],
	['Europe',        'Slovenia',         'C19'],
	['North America', 'USA',              'C20'],
	['North America', 'Mexico',           'C21'],
	['North America', 'Canada',           'C22'],
	['South America', 'Brazil',           'C23'],
	['South America', 'Argentina',        'C24'],
	['South America', 'Chile',            'C25'],
	['South America', 'Columbia',         'C26'],
	['South America', 'Peru',             'C27'],
	['South America', 'Bolivia',          'C28'],
	['South America', 'Honduras',         'C29'],
	['Asia',          'India',            'C30'],
	['Asia',          'China',            'C31'],
	['Asia',          'Korea',            'C32'],
	['Asia',          'Japan',            'C33'],
	['Asia',          'Thailand',         'C34'],
	['Asia',          'Malaysia',         'C35'],
	['Asia',          'Indonesia',        'C36'],
	['Asia',          'Iran',             'C37'],
	['Asia',          'Israel',           'C38'],
	['Asia',          'Saudi Arabia',     'C39'],
	['Africa',        'South Africa',     'C40'],
	['Africa',        'Egypt',            'C41'],
	['Africa',        'Marocco',          'C42'],
	['Africa',        'Tunesia',          'C43'],
	['Africa',        'Libia',            'C44'],
	['Africa',        'Sudan',            'C45'],
	['Africa',        'Congo',            'C46'],
	['Africa',        'Etiopia',          'C47'],
	['Oceania',       'Australia',        'C48'],
	['Oceania',       'New Zeland',       'C49']
];

const cross1 = [ 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t' ];

const cross2 = [ 'First', 'Second', 'Third', 'Fourth', 'Fifth'];

const years = [ '2014', '2015', '2016', '2017', '2018', '2019', '2020', '2021' ];
const quaters = [ 'Q1', 'Q2', 'Q3', 'Q4' ];

const length = countries.length 
	* cross1.length 
	* cross2.length 
	* years.length 
	* quaters.length;

console.log(length);

let data = {
	series: [
		{ name: 'Continents', type: 'categories', values: new Array(length) },
		{ name: 'Countries', type: 'categories', values: new Array(length) },
		{ name: 'Cross1', type: 'categories', values: new Array(length) },
		{ name: 'Cross2', type: 'categories', values: new Array(length) },
		{ name: 'Years', type: 'categories', values: new Array(length) },
		{ name: 'Quaters', type: 'categories', values: new Array(length) },
		{ name: 'Cont1', type: 'values', values: new Array(length) },
		{ name: 'Cont2', type: 'values', values: new Array(length) },
		{ name: 'Cont3', type: 'values', values: new Array(length) },
		{ name: 'Country code', type: 'categories', values: new Array(length) },
	]
};

let coMeans = (new Array(countries.length)).fill(0).map(x => rand() );

let cr1Means = new Array(cross1.length);
for (let i = 0; i < cr1Means.length; i++) cr1Means[i] = (1/(i+1)) * random(0.75,1);

let cr2Means = (new Array(cross2.length)).fill(0).map(x => rand() );

let cBase0 = perlin();
let cBase1 = perlin();
let cBase2 = perlin();
let qBase0 = (new Array(4)).fill(0).map(x => random(0.6,1) );
let qBase1 = (new Array(4)).fill(0).map(x => rand(0.75,1) );
let qBase2 = (new Array(4)).fill(0).map(x => rand(0.9,1) );

let cr2start = (new Array(cross2.length)).fill(0)
	.map(x => Math.floor(rand() * years.length * 0.75));
	console.log(cr2start);

let i = 0;
for (let iCountry = 0; iCountry < countries.length; iCountry++)
{
	let continent = countries[iCountry][0];
	let country = countries[iCountry][1];
	let countryCode = countries[iCountry][2];
	for (let icr1 = 0; icr1 < cross1.length; icr1++)
	{
		let cr1 = cross1[icr1];
		for (let icr2 = 0; icr2 < cross2.length; icr2++)
		{
			let cr2 = cross2[icr2];
			let c0 = perlin();
			let c1 = perlin();
			let c2 = perlin();
			for (let y = 0; y < years.length; y++)
			{
				let year = years[y];
				let exists = y > cr2start[icr2] || icr2 < cross2.length/3 ? 1 : 0;
				let q0 = (new Array(4)).fill(0).map(x => random(0.75,1) );
				let q1 = (new Array(4)).fill(0).map(x => rand(0.85,1) );
				let q2 = (new Array(4)).fill(0).map(x => rand(0.9,1) );
				for (let q = 0; q < quaters.length; q++)
				{
					let factor = coMeans[iCountry] * cr1Means[icr1] * cr2Means[icr2];
					let f0 = exists * factor * cBase0[y] * c0[y] * qBase0[q] * q0[q];
					let f1 = exists * factor * cBase1[y] * c1[y] * qBase1[q] * q1[q];
					let f2 = exists * factor * cBase2[y] * c2[y] * qBase2[q] * q2[q];
					data.series[0].values[i] = continent;
					data.series[1].values[i] = country;
					data.series[2].values[i] = cr1;
					data.series[3].values[i] = cr2;
					data.series[4].values[i] = year;
					data.series[5].values[i] = quaters[q];
					data.series[6].values[i] = Math.floor(scale(f0, 0, 4000000));
					data.series[7].values[i] = Math.floor(scale(f1, 0,50000));
					data.series[8].values[i] = Math.floor(scale(f2, -20000,400000));
					data.series[9].values[i] = countryCode;
					i++;
				}
			}
		}
	}
}

console.log('Data generated.');

export default data;