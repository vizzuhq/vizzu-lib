const fs = require('fs');

function copyHashes(failHashFile, refHashFile)
{
	const failHashData = JSON.parse(fs.readFileSync(failHashFile));
	const refHashData = JSON.parse(fs.readFileSync(refHashFile));
	
	for (const testFilename in failHashData.test) 
	{
		if (!refHashData.test[testFilename])
		{
			refHashData.test[testFilename] = {
				refs: [ failHashData.test[testFilename].refs[0] ]
			}
		}
		else if (refHashData.test[testFilename].refs[0] !== "")
			refHashData.test[testFilename].refs[0] = 
				failHashData.test[testFilename].refs[0]
	}
	
	fs.writeFileSync(refHashFile, JSON.stringify(refHashData, null, 4));	
}

copyHashes(
	"test_report/results/test_cases/test_cases.json",
	"test_cases/test_cases.json"
)
/*
copyHashes(
	"test_report/results/tests/style/style_tests.json",
	"tests/style_tests.json"
)*/
