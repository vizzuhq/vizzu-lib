#!/bin/bash

npm test -- --vizzu /dist/vizzu.min.js --hashes ALL --maxFailedImages 100 || TEST_FAILED=true
tar -czvf test/integration/test_report.tgz test/integration/test_report/ || TGZ_FAILED=true
gsutil cp 'test/integration/test_report.tgz' 'gs://vizzu-lib-main-sha/lib-$SHORT_SHA/test_report.tgz' || GSUTIL_FAILED=true
if [ "$TEST_FAILED" == "true" ] || [ "$TGZ_FAILED" == "true" ] || [ "$GSUTIL_FAILED" == "true" ]; then
    exit 1
fi
