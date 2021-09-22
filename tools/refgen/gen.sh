#!/bin/bash
# works with: typedoc 0.22.3, typedoc-plugin-markdown: 3.11.0  
npx typedoc --out ../../docs/reference ../../src/apps/weblib/js-api/vizzu.d.ts
# --hideBreadcrumbs true --hideInPageTOC true
