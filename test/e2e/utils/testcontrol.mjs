export function testController(chart, seekee) {
	const promise = Promise.resolve(chart)
	promise.activated = Promise.resolve({
		seek: (percent) => {
			seekee(parseFloat(percent) / 100)
		},
		pause: () => {},
		play: () => {}
	})
	return promise
}
