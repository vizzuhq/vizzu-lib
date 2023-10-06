const dataLoaded = import('../assets/data/music_data.js')
const mdChartLoaded = import('../assets/javascripts/mdchart.js')

Promise.all([dataLoaded, mdChartLoaded]).then((results) => {
  const data = results[0].default
  const MdChart = results[1].default
  const mdchart = new MdChart(data, 'tutorial')

  mdchart.create([
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: 'Label',
              channels: {
                y: { set: 'Popularity' },
                x: { set: 'Genres' }
              }
            }
          })
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                label: {
                  attach: 'Popularity'
                }
              }
            }
          })
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: 'Lightness - legend on'
            }
          })
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                lightness: {
                  attach: 'Popularity'
                }
              },
              legend: 'lightness'
            }
          })
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: 'Color'
            }
          })
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                lightness: {
                  set: null
                },
                color: {
                  attach: 'Genres'
                }
              },
              legend: 'color'
            }
          })
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: 'Size - change of geometry required'
            }
          })
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                size: {
                  set: 'Popularity'
                }
              },
              geometry: 'circle'
            }
          })
        }
      ]
    }
  ])
})
