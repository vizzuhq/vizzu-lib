export var data = {
    series: [
        { 
            name: 'Country_code',
            type: 'dimension',
            values: [
                'AT','BE','BG','CY','CZ','AT','BE','BG','CY','AT','BE','BG','CY','AT','AT','BE','BG','CY','CZ','DE','DK','EE','EL','ES','FI','FR','HR','HU'
                ]
            },
        { 
            name: 'Type',
            type: 'dimension',
            values: [ 
                'Joy factors','Joy factors','Joy factors','Joy factors','Joy factors','Joy factors','Joy factors','Joy factors','Joy factors','Joy factors','Joy factors','Joy factors','Joy factors','Joy factors','Country','Country','Country','Country','Country','Country','Country','Country','Country','Country','Country','Country','Country','Country'
            ]
        },
        { 
            name: 'Joy factors/Country',
            type: 'dimension',
            values: [ 
                'Terrorism','Terrorism','Terrorism','Terrorism','Terrorism','Poverty','Poverty','Poverty','Poverty','Climate change','Climate change','Climate change','Climate change','Political extremism','Austria','Belgium','Bulgaria','Cyprus','Czechia','Germany','Denmark','Estonia','Greece','Spain','Finland','France','Croatia','Hungary'
            ]
        },
        { 
            name: 'Year',
            type: 'dimension',
            values: [
                '2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020','2020'
            ]
        },
        { 
            name: 'Value 2 (+)',
            type: 'measure',
            values: [ 
                51,47,30,20,15,14,13,12,11,11,8,8,4,2,51,47,30,20,15,14,13,12,11,11,8,8,4,2
            ]
        },
        { 
            name: 'Value 5 (+/-)',
            type: 'measure',
            values: [ 
                51,47,30,-20,15,14,13,12,-11,-11,-8,-8,-4,2,51,47,30,-20,15,14,13,12,-11,-11,-8,-8,-4,2
            ]
        },
        { 
            name: 'index',
            type: 'measure',
            values: [ 
                0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,0.75,1,1,1,1,1,1,1,1,1,1,1,1,1,1
            ]
        }
    ]
};
