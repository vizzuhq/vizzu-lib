import UnPivot from '../../../../../src/apps/weblib/js-api/unpivot.js';
import testData from './unpivot.testdata.js';


test('assert_data_null_1', () => {
    expect(() => UnPivot.convert()).toThrow('Assert failed: data is not an object');
});

test('assert_data_null_2', () => {
    let data;
    expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object');
});

test('assert_data_list_1', () => {
    let data = [];
    expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object');
});


test('convert_3D_1', () => {
    UnPivot.convert(testData['3D_1'].input);
    expect(JSON.stringify(testData['3D_1'].input.series)).toMatch(JSON.stringify(testData['3D_1'].output.series));
});