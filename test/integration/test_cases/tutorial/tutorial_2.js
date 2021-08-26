import { data } from '/data/tutorial.js';
import { chart, wait } from '/scripts/interface.js';
import { generalStyles } from './tutorialstyles.js';
import { mergeDeep } from '/scripts/merge.js';


  let specificStyles = {

  };

  //let styles = mergeDeep(generalStyles, specificStyles);



  chart
    .animate({ 
//      style: styles
    })
    .then(() => {
      return chart.animate({ //1
        data: data,
        descriptor: {
          channels: {
            x: { attach: ['Timeseries'] },
            y: { attach: ['Values 1'], range: '0,1.1,%' },
            label: { attach: ['Values 1'] }
          },
          title: 'A simple column chart.',
          legend: null,
        },

        style: { plot: { marker: { label: { position: 'above', filter: 'lightness(0.1)' } } } },

      });
    })

        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({ //2
            descriptor: {
              channels: {
                y: { attach: ['Categ. Parent'] },
                color: { attach: ['Categ. Parent'] },
              },
              title: 'A cross-metric added to the y-axis and the color channel.',
              legend: "color",
            },

            style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } },

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //3
            descriptor: {
              channels: {},
              title: 'The geometric elements used can be changed to area...',
              geometry: 'area',
            },
          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //4
            descriptor: {
              title: '...or line...',
              geometry: 'line',
            },

            style: { plot: { marker: { label: { position: 'above', filter: 'lightness(0.1)' } } } },

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //5
            descriptor: {
              title: '...or back to rectangle.',
              geometry: 'rectangle',
            },

            style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } },

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //6
            descriptor: {
              channels: {
                y: { detach: ['Values 1'] },
                x: { attach: ['Values 1'], range: '0,1.1,%' },
              },
              legend: 'size', //workaround to hide legend
              title: 'Values can be grouped by the other axis too.',
            },
          });
        })
        .then(() => {
          return chart.animate({  //7
            descriptor: {
              channels: {
                x: { detach: 'Timeseries' },
              },
              title: 'Values can be grouped by the other axis too.',
            },

            style: { plot: { marker: { label: { position: 'above', filter: 'lightness(0.1)' } } } },

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //8
            descriptor: {
              channels: {
                x: { range: '0,1.333,%' },
              },
              title: 'Using polar coordinates instead of cartesian is also an option.',
              coordSystem: 'polar',
              legend: null,
            },

            style: { plot: {
              marker: { label: { position: 'center', filter: 'lightness(0.8)' } } ,
              axis: { label: { color: '#ffffff00' }}}}, // Magyarázat

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //9
            descriptor: {
              channels: {
                x: { attach: 'Timeseries' },
                lightness: { attach: ["Values 2"] },
              },
              title: 'Values can be added to the lightness channel.',
              legend: "lightness",
            },
          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //10
            descriptor: {
              channels: {
                y: { detach: ['Categ. Parent'] },
                x: { detach: ['Values 1', 'Timeseries'] },
                size: { attach: ['Values 3', 'Timeseries'] },
              },
              title: 'Charts can also exist without any data on the axes.',
              legend: 'shape', //workaround to hide legend
              coordSystem: 'cartesian',
              legend: null,
            },

            style: { plot: {
              axis: { label: { color: '#abababFF'}}}}, // Magyarázat

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //11
            descriptor: {
              channels: {
                size: { attach: ["Categ. Child"] },
              },
              title: 'More categorical data series can be on the same channel.',
            },
          });
        })
        .then(() => { return wait(2 * 1000); })
        .then(() => {
          return chart.animate({  //12
            descriptor: {
              channels: {
                size: { detach: ['Values 3', 'Timeseries', 'Categ. Child'] },
                y: { attach: ['Categ. Parent', 'Categ. Child', 'Values 1'], range: '0,1.1,%' },
                x: { attach: ['Timeseries'] },
                lightness: { detach: ['Values 2'] },
              },
              legend: "color",
              title: 'Getting back to the stacked bar chart in one step.',
            },
          });
        })
        .then(() => {
          return chart.animate({  //13
            descriptor: {
              channels: {
                y: { detach: ['Categ. Child'] },
              },
              title: 'Getting back to the stacked bar chart in one step.',
            },
          });
        })
        .then(() => { return wait(2 * 1000); })
        .then(() => {
          return chart.animate({  //14
            descriptor: {
              channels: {
                y: { detach: ['Categ. Parent'] },
                x: { attach: ['Categ. Parent'] },
              },
              title: 'Data can be grouped...',
            },

            style: { plot: { marker: { label: { position: 'above', filter: 'lightness(0.1)' } } } },

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //15
            descriptor: {
              channels: {
                x: { detach: ['Categ. Parent'] },
                y: { attach: ['Categ. Parent'] },
              },
              title: '...or stacked, by putting a category from one axis to the other.',
            },

            style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } },

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //16
            descriptor: {
              title: 'Comparing ratios is just another option out of many more...',
              align: 'stretch'
            },
          });
        }).


        catch((err) => {
          console.log(err);
        });

 

