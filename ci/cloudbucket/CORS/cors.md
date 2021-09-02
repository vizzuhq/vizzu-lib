# Configuring CORS on a bucket

https://cloud.google.com/storage/docs/configuring-cors

Use the gsutil cors command to configure CORS on a bucket:
```
gsutil cors set JSON_FILE_NAME.json gs://BUCKET_NAME
```
Where
* JSON_FILE_NAME is the path to cors.json JSON file you created.
* BUCKET_NAME is the name of the bucket. For example, my-bucket.