#!/bin/bash
# works with: typedoc 0.22.3, typedoc-plugin-markdown: 3.11.0  
npx typedoc --readme ../../docs/content/reference/reference.md --out ../../docs/reference ../../src/apps/weblib/js-api/vizzu.d.ts
# --hideBreadcrumbs true --hideInPageTOC true
