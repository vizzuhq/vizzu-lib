# Building the Documentation Site

In vizzu-lib repo:

* Generate static thumbnails using integration test runner

      cd <repo>/test/integration
      node vizzutest.js test_cases/web_content/sample_static/*/*
      
* Generate video thumbnails

      cd <repo>/test/integration/modules/videorecorder
      node generate.js ../../test_cases/web_content/templates/*
      rm -rf resized # remove output folder from previous run
      node resize.js
      
* Generate reference doc

      cd <repo>/tools/refgen
      node typedoc --out ../../docs/reference ../../src/apps/weblib/js-api/vizzu.d.ts

* Generate documentation site

      cd <repo>/docs/generator
      node gendoc.js
   
# Publishing the Documentation Site

In vizzu-lib-doc repo:

* Copy generated site

      cd <repo>/docs/<version>
      cp -r <vizzu-lib>/docs/* .
      rm -rf generator
      git commit -a -m '<version>'
      git push
