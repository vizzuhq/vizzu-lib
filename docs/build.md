# Building thumbnails for the Documentation Site

* Generate static thumbnails using integration test runner

      cd <repo>/test/integration
      node test.js --images ALL test_cases/web_content/sample_static/*/*
      
* Generate video thumbnails

      cd <repo>/test/integration/modules/videorecorder
      node generate.js ../../test_cases/web_content/templates/*
      rm -rf resized # remove output folder from previous run
      node resize.js
