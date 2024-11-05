const renderingUpdate = 'chart.feature.rendering.update()'
const unknownEvent = "if (!error.toString().includes('unknown event handler')) throw error"

const image = new Image()
image.src =
	'data:image/gif;base64,R0lGODlhAwACAPIAAJLf6q/i7M/r8un0+PT6+/n8/QAAAAAAACH5BAQAAAAALAAAAAADAAIAAAMEWBMkkAA7'

export default [
	[
		{
			name: '../assets/setup/setup_c',
			returnOriginal: true,
			replace: [
				[
					'chart.animate',
					`try { chart.off('click', assets.eventHandler) } catch (error) { ${unknownEvent} }; chart = chart.animate`
				],
				['config: {', "config: {title: 'Click event added to markers',"]
			]
		},
		{
			name: '01',
			returnOriginal: true,
			replace: [['clickHandler)', `assets.eventHandler);${renderingUpdate}`]],
			assets: { eventHandler: (event) => alert(JSON.stringify(event.target)) }
		}
	],
	[
		{
			name: '02_a',
			returnOriginal: true
		},
		{
			name: '02_b',
			returnOriginal: true,
			replace: [['labelDrawHandler)', `assets.eventHandler);${renderingUpdate}`]],
			assets: {
				eventHandler: (event) => {
					event.renderingContext.fillStyle =
						event.target.value === 'Jazz' ? 'red' : 'gray'
				}
			}
		}
	],
	[
		{
			name: '03_a',
			returnOriginal: true
		},
		{
			name: '03_b',
			returnOriginal: true,
			replace: [['logoDrawHandler)', `assets.eventHandler);${renderingUpdate}`]],
			assets: {
				eventHandler: (event) => {
					event.preventDefault()
				}
			}
		}
	],
	[
		{
			name: '04_a',
			returnOriginal: true
		},
		{
			name: '04_b',
			returnOriginal: true,
			replace: [['backgroundImageHandler)', `backgroundImageHandler);${renderingUpdate}`]],
			assets: {
				eventHandler: (event) => {
					event.renderingContext.drawImage(
						image,
						0,
						0,
						event.detail.rect.size.x,
						event.detail.rect.size.y
					)
					event.preventDefault()
				}
			}
		}
	]
]
