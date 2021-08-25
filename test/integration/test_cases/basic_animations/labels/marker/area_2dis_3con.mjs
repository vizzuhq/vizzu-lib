import { data } from '/test/integration/test_data/chart_types_eu.js';
data.filter = record =>
record.Country != 'Germany' &&
record.Country != 'Italy' &&
record.Country != 'Hungary' &&
record.Country != 'Netherlands' &&
record.Country != 'Estonia' &&
record.Country != 'Belgium' &&
record.Country != 'Sweden' &&
record.Country != 'Poland' &&
record.Country != 'Malta' &&
record.Country != 'Romania' &&
record.Country != 'Greece' &&
record.Country != 'Slovakia' &&
record.Country != 'Ireland' &&
record.Country != 'Lithuania' &&
record.Country != 'Croatia' &&
record.Country != 'Slovenia' &&
record.Country != 'Portugal' &&
record.Country != 'Finland' &&
record.Country != 'United Kingdom';

const testSteps = [
    chart => chart.animate(
        {   data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Country', 'Positive nums' ], range: '0,1.1,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] },
                    label: { attach: ['Positive nums'] }
                },
                title: 'Area Marker Label Positions - Center',
                geometry: 'area'
            },
            style:
            {
                plot:
                {   paddingLeft: 120,
                    marker:
                    {
                        label:
                        {
                            position: 'center'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Top',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'top',
                            orientation: 'horizontal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Left',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'left'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Bottom',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'bottom'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Right',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'right'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Center/Horizontal',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'center'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Center/Tangential',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'tangential'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Center/Normal',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'normal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Center/Vertical',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'vertical'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                channels:
                {
                    y: { detach: ['Positive nums'] },
                    y: { attach: ['Pos_small'] },
                    label: { detach: ['Positive nums'] },
                    label: { attach: ['Pos_small'] },
                },
                title: 'Change Continuous'
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'top',
                            orientation: 'horizontal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                channels:
                {
                    y: { detach: ['Pos_small'] },
                    y: { attach: ['Positive nums'] },
                    label: { detach: ['Pos_small'] },
                    label: { attach: ['Positive nums'] },
                },
                title: 'Polar Coordinate',
                coordSystem: 'polar'
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'horizontal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Left',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'left'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Bottom',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'bottom'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Right',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'right'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Center/Horizontal',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'center',
                            orientation: 'horizontal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Top/Normal',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            position: 'top',
                            orientation: 'normal'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Top/Vertical',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'vertical'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Top/Tangential',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            orientation: 'tangential'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Top/Angle',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            angle: 0.5 * 3.14
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Area Marker Label Positions - Top/Angle',
            },
            style: {
                plot: {
                    marker: {
                        label: {
                            angle: null
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;