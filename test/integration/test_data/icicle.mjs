export var data = {
  series: [
    {
      name: 'Country_code',
      type: 'dimension',
      values: [
        'AT', 'BE', 'BG', 'CY', 'CZ', 'DE', 'DK', 'EE', 'EL', 'ES',
        'FI', 'FR', 'HR', 'HU', 'AT', 'BE', 'BG', 'CY', 'CZ', 'AT',
        'BE', 'BG', 'CY', 'AT', 'BE', 'BG', 'CY', 'AT'
      ]
    },
    {
      name: 'Type',
      type: 'dimension',
      values: [
        'Country', 'Country', 'Country', 'Country', 'Country', 'Country',
        'Country', 'Country', 'Country', 'Country', 'Country', 'Country',
        'Country', 'Country', 'Joy factors',  'Joy factors',  'Joy factors',  'Joy factors',
        'Joy factors',  'Joy factors',  'Joy factors',  'Joy factors',  'Joy factors',  'Joy factors',
        'Joy factors',  'Joy factors',  'Joy factors',  'Joy factors'
      ]
    },
    {
      name: 'Joy factors/Country',
      type: 'dimension',
      values: [
        'Austria',        'Belgium',
        'Bulgaria',       'Cyprus',
        'Czechia',        'Germany',
        'Denmark',        'Estonia',
        'Greece',         'Spain',
        'Finland',        'France',
        'Croatia',        'Hungary',
        'Terrorism',      'Terrorism',
        'Terrorism',      'Terrorism',
        'Terrorism',      'Poverty',
        'Poverty',        'Poverty',
        'Poverty',        'Climate change',
        'Climate change', 'Climate change',
        'Climate change', 'Political extremism'
      ]
    },
    {
      name: 'Year',
      type: 'dimension',
      values: [
        '<y>2020', '<y>2020', '<y>2020', '<y>2020', '<y>2020', '<y>2020',
        '<y>2020', '<y>2020', '<y>2020', '<y>2020', '<y>2020', '<y>2020',
        '<y>2020', '<y>2020', '<y>2020', '<y>2020', '<y>2020', '<y>2020',
        '<y>2020', '<y>2020', '<y>2020', '<y>2020', '<y>2020', '<y>2020',
        '<y>2020', '<y>2020', '<y>2020', '<y>2020'
      ]
    },
    {
      name: 'Value 2 (+)',
      type: 'measure',
      values: [
        '51', '47', '30', '20', '15', '14', '13', '12', '11', '11',
        '8',  '8',  '4',  '2',  '51', '47', '30', '20', '15', '14',
        '13', '12', '11', '11', '8',  '8',  '4',  '2'
      ]
    },
    {
      name: 'Value 5 (+/-)',
      type: 'measure',
      values: [
        '51', '47', '30',  '-20', '15', '14', '13', '12',  '-11', '-11',
        '-8', '-8', '-4',  '2',   '51', '47', '30', '-20', '15',  '14',
        '13', '12', '-11', '-11', '-8', '-8', '-4', '2'
      ]
    },
    {
      name: 'index',
      type: 'measure',
      values: [
        '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
        '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
        '1', '1', '1', '1', '1', '1', '1', '1'
      ]
    }
  ]
};