import { data } from '/data/tutorial.js';
import { chart, wait } from '/scripts/interface.js';
import { generalStyles } from '../tutorial/tutorialstyles.js';
import { mergeDeep } from '/scripts/merge.js';
//

  let specificStyles = {

  };

  let styles = mergeDeep(generalStyles, specificStyles);





  chart
    .animate({ //State 0 - Creating an empty chart
     style: styles
    })
    .then(() => {
      return chart.animate({ //State 1 - Drawing a static bar chart
        data: data,
        descriptor: {
          channels: {
            x: { attach: ['Timeseries'] }, // Adding the Timeseries to the x-axis
            y: { attach: ['Values 1'], range: '0,1.1,%' }, // Adding the Values 1 to the y-axis & setting the range from 0 to 110% of the biggest value in the series
            label: { attach: ['Values 1'] } //Adding the same data series to the label channel, so that the values will be shown on the bars.
          },
          title: 'A simple column chart.', //Setting the chart title
          legend: null, // Turning off the legend
        },

        style: { plot: { marker: { label: { position: 'above', filter: 'lightness(0.1)' } } } }, //Set the labels to show on top of the bars.

      });
    })

        .then(() => { return wait(1 * 1000); }) //Wait a second before the next state
        .then(() => {
          return chart.animate({ // State 2 - The first animation
            descriptor: {
              channels: {
                y: { attach: ['Categ. Parent'] }, //Adding a categorical data to the y-axis
                color: { attach: ['Categ. Parent'] }, //Adding the same date series to the color channel, resulting in a stacked bar chart with different colors used for the different categories on the y axis.
              },
              title: 'A cross-metric added to the y-axis and the color channel.',
              legend: "color", //Switching on the legend
            },

            style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } }, //Change the labels' position to the center of the markers so that all three are visible in each bar.

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //State 3 - Changing geometries
            descriptor: {
              channels: {},
              title: 'The geometric elements used can be changed to area...',
              geometry: 'area', //Setting the geometry to area, resulting in an area chart.
            },
          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //State 4 - Line chart
            descriptor: {
              title: '...or line...',
              geometry: 'line', //Setting the geometry to line, resulting in a line chart.
            },

            style: { plot: { marker: { label: { position: 'above', filter: 'lightness(0.1)' } } } }, //labels once again need to be above the makers since we changed the geometry

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //State 5 - Column chart
            descriptor: {
              title: '...or back to rectangle.',
              geometry: 'rectangle', //Setting the geometry to rectangle, resulting in a column chart.
            },

            style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } },

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //State 6 - Orientation change
            descriptor: {
              channels: {
                y: { detach: ['Values 1'] },                    //The values are taken down from the y-axis...
                x: { attach: ['Values 1'], range: '0,1.1,%' },  //And are put on the x-axis resulting in regrouping the same values by the categories on this axis.
              },
              legend: null, //Hiding the legend
              title: 'Values can be grouped by the other axis too.',
            },
          });
        })
        .then(() => {
          return chart.animate({ //State 7 - Taking down the unnecessary category from the x-axis
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
          return chart.animate({  //State 8 - Switching to polar coordinates
            descriptor: {
              channels: {
                x: { range: '0,1.333,%' },
              },
              title: 'Using polar coordinates instead of cartesian is also an option.',
              coordSystem: 'polar', //This is it
              legend: null,
            },

            style: { plot: {
              marker: { label: { position: 'center', filter: 'lightness(0.8)' } } ,
              axis: { label: { color: '#ffffff00' }}}}, // Hiding the axis labels by turning them transparent as in polar the x-axis is rounded so all labels are at one place

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //State 9 - Lightness channel
            descriptor: {
              channels: {
                x: { attach: 'Timeseries' }, //Adding Timeseries once again to the x-axis.
                lightness: { attach: ["Values 2"] }, //Adding Values 2 to the Lightness channel
              },
              title: 'Values can be added to the lightness channel.',
              legend: "lightness",
            },
          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //State 10 - Treemap
            descriptor: {
              channels: {
                y: { detach: ['Categ. Parent'] }, //Taking everything off of the axes
                x: { detach: ['Values 1', 'Timeseries'] },
                size: { attach: ['Values 3', 'Timeseries'] }, //And adding data to the size channel instead.
              },
              title: 'Charts can also exist without any data on the axes.',
              legend: null,
              coordSystem: 'cartesian', //Switching back to cartesian coordinates from polar
            },

            style: { plot: {
              axis: { label: { color: '#abababFF'}}}}, // Turning the axis labels visible again - they can not be seen on this particular state but will be used later on.

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //State 11 - Parent and child
            descriptor: {
              channels: {
                size: { attach: ["Categ. Child"] }, //Adding one more categorical series to the size creates splits each parent category to its children.
              },
              title: 'More categorical data series can be on the same channel.',
            },
          });
        })
        .then(() => { return wait(2 * 1000); })
        .then(() => {
          return chart.animate({  //State 12 - Column chart
            descriptor: {
              channels: {
                size: { detach: ['Values 3', 'Timeseries', 'Categ. Child'] }, //Taking off the unnecessary series from the size channel
                y: { attach: ['Categ. Parent', 'Categ. Child', 'Values 1'], range: '0,1.1,%' }, //And putting them back on the axes
                x: { attach: ['Timeseries'] },
                lightness: { detach: ['Values 2'] },
              },
              legend: "color",
              title: 'Getting back to the stacked bar chart in one step.',
            },
          });
        })
        .then(() => {
          return chart.animate({  //State 13 - Removing an category we don't need any more
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
          return chart.animate({  //State 14 - Grouping
            descriptor: {
              channels: {
                y: { detach: ['Categ. Parent'] }, //Taking one categorical series from the y-axis...
                x: { attach: ['Categ. Parent'] }, //...to the x-axis.
              },
              title: 'Data can be grouped...',
            },

            style: { plot: { marker: { label: { position: 'above', filter: 'lightness(0.1)' } } } },

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //State 15 - Stacking
            descriptor: {
              channels: {
                x: { detach: ['Categ. Parent'] }, //Putting the category back to the original axis
                y: { attach: ['Categ. Parent'] },
              },
              title: '...or stacked, by putting a category from one axis to the other.',
            },

            style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } },

          });
        })
        .then(() => { return wait(1 * 1000); })
        .then(() => {
          return chart.animate({  //State 16 - Percentage view
            descriptor: {
              title: 'Comparing ratios is just another option out of many more...',
              align: 'stretch' //Switching from nominal comparison to ratios or percentages is done only via streching all the bars to the max value.
            },
          });
        }).


        catch((err) => {
          console.log(err);
        });

 //

