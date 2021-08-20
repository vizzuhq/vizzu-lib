export default class ImgDiff
{
	static compare(type, act, ref, w, h)
	{
		let match = true;
		const diffData = new Uint8ClampedArray(w * h * 4);
		for (let i = 0; i < diffData.length; i+=4)
		{
			let same = ImgDiff.isSame(act, ref, i);
			if (!same) match = false;

			if (type == 'move')
				ImgDiff.moveDetection(act, ref, diffData, i);
			else
				ImgDiff.changeDetection(act, ref, diffData, i);

			let gray = ImgDiff.gray(act, ref, i);
			let c = (0.85 + 0.15 * gray);
			diffData[i+0] *= c;
			diffData[i+1] *= c;
			diffData[i+2] *= c;

			diffData[i+3] = 255;
		}
		return { match, diffData };
	}

	static moveDetection(act, ref, diffData, i)
	{
		let dif = ImgDiff.signDif(act, ref, i);
		let dr = dif.pos == 0 ? 0 : 0.2 + 0.8 * dif.pos;
		let db = dif.neg == 0 ? 0 : 0.2 + 0.8 * dif.neg;
		diffData[i+0] = (1 - db) * 255;
		diffData[i+1] = (1 - db) * (1 - dr) * 255;
		diffData[i+2] = (1 - dr) * 255;	
	}

	static changeDetection(act, ref, diffData, i)
	{
		let difs = ImgDiff.absDifs(act, ref, i);
		diffData[i+0] = difs.r == 0 ? 255 : (0.75 * (255-difs.r));
		diffData[i+1] = difs.g == 0 ? 255 : (0.75 * (255-difs.g));
		diffData[i+2] = difs.b == 0 ? 255 : (0.75 * (255-difs.b));
	}
	
	static signDif(act, ref, i) {
		let d = ImgDiff.difs(act, ref, i); 
		let pos = (ImgDiff.posdif(d.r) + ImgDiff.posdif(d.g) + ImgDiff.posdif(d.b) + ImgDiff.posdif(d.a)) / (4*255);
		let neg = (ImgDiff.negdif(d.r) + ImgDiff.negdif(d.g) + ImgDiff.negdif(d.b) + ImgDiff.negdif(d.a)) / (4*255);
		return { pos, neg };
	}

	static absDifs(a, b, i) {
		let d = ImgDiff.difs(a, b, i); 
		return { 
			r: Math.abs(d.r),
			g: Math.abs(d.g),
			b: Math.abs(d.b),
			a: Math.abs(d.a)
		};
	}

	static difs(act, ref, i) {
		let r = act[i + 0] - ref[i + 0];
		let g = act[i + 1] - ref[i + 1];
		let b = act[i + 2] - ref[i + 2];
		let a = act[i + 3] - ref[i + 3];
		return { r, g, b, a };
	}
	
	static gray(act, ref, i) {
		return (act[i + 0] + ref[i + 0] 
			  + act[i + 1] + ref[i + 1] 
			  + act[i + 2] + ref[i + 2] 
			  + act[i + 3] + ref[i + 3]) / 8 / 255;
	}

	static isSame(act, ref, i) {
		return (act[i + 0] == ref[i + 0] 
			 && act[i + 1] == ref[i + 1] 
			 && act[i + 2] == ref[i + 2] 
			 && act[i + 3] == ref[i + 3]);
	}

	static posdif(x) { 
		return x > 0 ? x : 0; 
	}
	
	static negdif(x) { 
		return x < 0 ? -x : 0; 
	}
	
}
