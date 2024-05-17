class ImgDiff {
	constructor(frame, frameRef, difCanvas) {
		this.frame = frame
		this.frameRef = frameRef
		this.difCanvas = difCanvas
	}

	getDiff() {
		const doc = this.frame.contentWindow.document
		const docRef = this.frameRef.contentWindow.document
		if (doc.vizzuImgData && docRef.vizzuImgData && doc.vizzuImgIndex === docRef.vizzuImgIndex) {
			const { width: w, height: h, data } = doc.vizzuImgData
			const res = ImgDiff.compare('move', data, docRef.vizzuImgData.data, w, h)

			const dif = new ImageData(res.diffData, w, h)
			this.difCanvas.width = 800
			this.difCanvas.height = 500
			const ctx = this.difCanvas.getContext('2d')
			ctx.clearRect(0, 0, w, h)
			ctx.putImageData(dif, 0, 0)
			doc.vizzuImgData = docRef.vizzuImgData = undefined
			this.difCanvas.style.border = `1px solid ${res.match ? 'green' : 'red'}`
		}

		setTimeout(() => this.getDiff(), 100)
	}

	static compare(type, act, ref, w, h) {
		let match = true
		const diffData = new Uint8ClampedArray(w * h * 4)
		for (let i = 0; i < diffData.length; i += 4) {
			const same = ImgDiff.isSame(act, ref, i)
			if (!same) match = false

			if (type === 'move') ImgDiff.moveDetection(act, ref, diffData, i)
			else ImgDiff.changeDetection(act, ref, diffData, i)

			const gray = ImgDiff.gray(act, ref, i)
			const c = 0.85 + 0.15 * gray
			diffData[i + 0] *= c
			diffData[i + 1] *= c
			diffData[i + 2] *= c

			diffData[i + 3] = 255
		}
		return { match, diffData }
	}

	static moveDetection(act, ref, diffData, i) {
		const dif = ImgDiff.signDif(act, ref, i)
		const dr = dif.pos === 0 ? 0 : 0.2 + 0.8 * dif.pos
		const db = dif.neg === 0 ? 0 : 0.2 + 0.8 * dif.neg
		diffData[i + 0] = (1 - db) * 255
		diffData[i + 1] = (1 - db) * (1 - dr) * 255
		diffData[i + 2] = (1 - dr) * 255
	}

	static changeDetection(act, ref, diffData, i) {
		const difs = ImgDiff.absDifs(act, ref, i)
		diffData[i + 0] = difs.r === 0 ? 255 : 0.75 * (255 - difs.r)
		diffData[i + 1] = difs.g === 0 ? 255 : 0.75 * (255 - difs.g)
		diffData[i + 2] = difs.b === 0 ? 255 : 0.75 * (255 - difs.b)
	}

	static signDif(act, ref, i) {
		const d = ImgDiff.difs(act, ref, i)
		const pos =
			(ImgDiff.posdif(d.r) +
				ImgDiff.posdif(d.g) +
				ImgDiff.posdif(d.b) +
				ImgDiff.posdif(d.a)) /
			(4 * 255)
		const neg =
			(ImgDiff.negdif(d.r) +
				ImgDiff.negdif(d.g) +
				ImgDiff.negdif(d.b) +
				ImgDiff.negdif(d.a)) /
			(4 * 255)
		return { pos, neg }
	}

	static absDifs(a, b, i) {
		const d = ImgDiff.difs(a, b, i)
		return {
			r: Math.abs(d.r),
			g: Math.abs(d.g),
			b: Math.abs(d.b),
			a: Math.abs(d.a)
		}
	}

	static difs(act, ref, i) {
		const r = act[i + 0] - ref[i + 0]
		const g = act[i + 1] - ref[i + 1]
		const b = act[i + 2] - ref[i + 2]
		const a = act[i + 3] - ref[i + 3]
		return { r, g, b, a }
	}

	static gray(act, ref, i) {
		return (
			(act[i + 0] +
				ref[i + 0] +
				act[i + 1] +
				ref[i + 1] +
				act[i + 2] +
				ref[i + 2] +
				act[i + 3] +
				ref[i + 3]) /
			8 /
			255
		)
	}

	static isSame(act, ref, i) {
		return (
			act[i + 0] === ref[i + 0] &&
			act[i + 1] === ref[i + 1] &&
			act[i + 2] === ref[i + 2] &&
			act[i + 3] === ref[i + 3]
		)
	}

	static posdif(x) {
		return x > 0 ? x : 0
	}

	static negdif(x) {
		return x < 0 ? -x : 0
	}
}

export default ImgDiff
