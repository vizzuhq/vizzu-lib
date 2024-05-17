#!/bin/bash

SHORT_SHA="$1"
npm test -- --vizzu /dist/vizzu.min.js --Werror sameref --hashes ALL --maxFailedImages 100 || TEST_FAILED=true
tar -czvf test/e2e/test_report.tgz test/e2e/test_report/ || TGZ_FAILED=true
gsutil cp "test/e2e/test_report.tgz" "gs://vizzu-lib-main-sha/lib-$SHORT_SHA/test_report.tgz" || GSUTIL_FAILED=true
if [ "$TEST_FAILED" == "true" ] || [ "$TGZ_FAILED" == "true" ] || [ "$GSUTIL_FAILED" == "true" ]; then
    exit 1
fi
