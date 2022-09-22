import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    record.Country == 'Austria' ||
                    record.Country == 'Belgium' ||
                    record.Country == 'Bulgaria' ||
                    record.Country == 'Cyprus' ||
                    record.Country == 'Germany' ||
                    record.Country == 'Denmark' ||
                    record.Country == 'Estonia' ||
                    record.Country == 'Spain' ||
                    record.Country == 'France' 
            }),
            config: {
                channels: {
                    x: { set: 'Year' },
                    y: { set: 'Value 2 (+)' },
                    color: { set: null }
                },
                title: 'Area',
                geometry: 'area'
            },
            style: {
                plot: {
                    paddingLeft: '8em',
                    yAxis: {
                        label: { paddingRight: '0.8em' }
                    },
                    xAxis: {
                        label: { paddingTop: '0.8em' },
                        title: { paddingTop: '2.4em' }
                    }
                },
                title: {
                    fontSize: '2em'
                }
            }
        }
        ),

        chart => chart.animate({
            config: {
                channels: {
                    x: { set: 'Year' },
                    y: { set: ['Value 2 (+)', 'Country'] },
                    color: { set: 'Country' }
                },
                title: 'Stacked Area / Drill down(?), + Discrete(?)',
                geometry: 'area'
            },
            style: {
                plot: {
                    paddingLeft: '0em',
                    yAxis: {
                        label: { paddingRight: '0.8em' }
                    },
                    xAxis: {
                        label: { paddingTop: '0.8em' }
                    }
                }
            }
        },
           {
                coordSystem: {
                    delay: 0,
                    duration: 1,
                },
                geometry: { 
                    delay: 0, 
                    duration: 0.75, 
    //                easing: 'linear' 
                },
                x: {
                    delay: 0.5,
                    duration: 0.5,
    //                easing: 'ease-out'
                }, 
                y: {
                    delay: 0,
                    duration: 0.75,
    //                easing: 'cubic-bezier(65,0,65,1)'
                }
            }
        ),

        chart => chart.animate({
            config: {
                channels: {
                    x: { set: 'Year' },
                    y: { set: 'Value 2 (+)' },
                    color: { set: 'Country' }
                },
                title: 'Line Chart / Comparison(?), Components(?), ',
                geometry: 'line'
            },
            style: {
                plot: {
                    paddingLeft: '0em',
                    yAxis: {
                        label: { paddingRight: '0.8em' }
                    },
                    xAxis: {
                        label: { paddingTop: '0.8em' }
                    }
                }
            }
        },
        {
                 delay: 1,
         }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)', 'Country'] },
                color: { set: 'Country' }
            },
            title: 'Trellis Area / Trellis(?), Components(?), Part-to-whole(?) (sort???)',
            geometry: 'area',
            split: true
        }
    },
    {
             delay: 1,
     }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)', 'Country'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Area / Trellis off(?), Merge(?)',
            geometry: 'area',
            split: false
        },
        style: {
            plot: {
                paddingLeft: '0em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
             delay: 1,
     }
    ),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)', 'Country'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Area / Ratio(?)',
            geometry: 'area',
            align: 'stretch'
        },
        style: {
            plot: {
                paddingLeft: '0em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
       {
            coordSystem: {
                delay: 0,
                duration: 1,
            },
            geometry: { 
                delay: 0, 
                duration: 0.75, 
//                easing: 'linear' 
            },
            x: {
                delay: 0.5,
                duration: 0.5,
//                easing: 'ease-out'
            }, 
            y: {
                delay: 0,
                duration: 0.75,
//                easing: 'cubic-bezier(65,0,65,1)'
            }
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)', 'Country'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Area / Ratio off(?)',
            geometry: 'area',
            align: 'min',
            split: false
        },
        style: {
            plot: {
                paddingLeft: '0em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
             delay: 1,
     }
    ),



    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)', 'Country'], range: { min: '0%', max: '100%' } },
                color: { set: 'Country' }
            },
            title: 'Bar / Sum(?)',
            geometry: 'area',
            align: 'min',
            split: true
        },
        style: {
            plot: {
                paddingLeft: '0em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
             delay: 1, duration: 1
     }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year', 'Value 2 (+)'] },
                y: { set: ['Country'] },
                color: { set: null }
            },
            title: 'Bar / Sum(?)',
            geometry: 'rectangle',
            align: 'min',
            split: false
        },
        style: {
            plot: {
                paddingLeft: '8em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
         delay: 0,
         duration: 1,
         geometry: { 
             delay: 0, 
             duration: 0.5
         },
         coordSystem: {
            delay: 0,
            duration: 0.5
        }, 
         x: {
             delay: 0,
             duration: 1
         }, 
         y: {
             delay: 0,
             duration: 0.5,
//                easing: 'cubic-bezier(65,0,65,1)'
         }
     }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 2 (+)'] },
                y: { set: ['Country'] },
                color: { set: null },
                label: { set: 'Value 2 (+)' }

                
            },
            title: 'Bar / Sum(?)',
            geometry: 'rectangle',
            align: 'min',
            split: false
        },
        style: {
            plot: {
                paddingLeft: '8em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
         delay: 0,
         duration: 0.3
     }
    ),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year', 'Value 2 (+)'] },
                y: { set: ['Country'] },
                color: { set: null },
                label: { set: null }
            },
            title: 'Bar / Sum(?)',
            geometry: 'rectangle',
            align: 'min',
            split: false
        },
        style: {
            plot: {
                paddingLeft: '8em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
         delay: 1,
         duration: 1,
         geometry: { 
             delay: 0, 
             duration: 1
         },
         coordSystem: {
            delay: 0,
            duration: 0.5
        }, 
         x: {
             delay: 0,
             duration: 1
         }, 
         y: {
             delay: 0,
             duration: 0.5,
//                easing: 'cubic-bezier(65,0,65,1)'
         }
     }
    ),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)', 'Country'], range: { min: '0%', max: '110%' } },
                color: { set: 'Country' }
            },
            title: 'Area / Time distribution(?)',
            geometry: 'area',
            align: 'min',
            split: true
        },
        style: {
            plot: {
                paddingLeft: '0em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
             delay: 0, duration: 1
     }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)', 'Country'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Area / Time distribution(?)',
            geometry: 'area',
            align: 'min',
            split: false
        },
        style: {
            plot: {
                paddingLeft: '0em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
             delay: 0,
     }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)'] },
                color: { set: null }
            },
            title: 'Area / Total timeseries(?)',
            geometry: 'area',
            align: 'min',
            split: false
        },
        style: {
            plot: {
                paddingLeft: '8em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
             delay: 1,
     }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)', 'Country'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Area / Time distribution(?)',
            geometry: 'area',
            align: 'min',
            split: false
        },
        style: {
            plot: {
                paddingLeft: '0em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
             delay: 1,
     }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: '16 Lollipop / Distribution(?)',
            geometry: 'circle',
            align: 'min',
            split: false
        },
        style: {
            plot: {
                paddingLeft: '0em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
         delay: 1,
         duration: 2,
         geometry: { 
             delay: 0, 
             duration: 0.75
         },
         coordSystem: {
            delay: 0,
            duration: 0.5
        }, 
         x: {
             delay: 0.5,
             duration: 1
         }, 
         y: {
             delay: 0.5,
             duration: 1,
//                easing: 'cubic-bezier(65,0,65,1)'
         }
     }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Value 2 (+)', 'Country'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Area / Sum(?), Sum timeseries(?)',
            geometry: 'area',
            align: 'min',
            split: false
        },
        style: {
            plot: {
                paddingLeft: '0em',
                yAxis: {
                    label: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' }
                }
            }
        }
    },
    {
             delay: 1,
     }
    ),


    chart => chart.feature('tooltip',true)
];

export default testSteps;