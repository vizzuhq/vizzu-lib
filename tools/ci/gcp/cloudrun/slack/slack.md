# Configuring Slack notifications

https://cloud.google.com/build/docs/configuring-notifications/configure-slack

```
gsutil cp slackconfig gs://vizzu-lib-main/slack/slackconfig

gcloud run deploy ci-results --image=us-east1-docker.pkg.dev/gcb-release/cloud-build-notifiers/slack:latest --no-allow-unauthenticated --update-env-vars=CONFIG_PATH=gs://vizzu-lib-main/slack/slackconfig,PROJECT_ID=418279184538

gcloud projects add-iam-policy-binding vizzu-ci --member=serviceAccount:418279184538-compute@developer.gserviceaccount.com --role=roles/iam.serviceAccountTokenCreator

gcloud iam service-accounts create cloud-run-pubsub-invoker --display-name "Cloud Run Pub/Sub Invoker"

gcloud run services add-iam-policy-binding ci-results --member=serviceAccount:cloud-run-pubsub-invoker@vizzu-ci.iam.gserviceaccount.com --role=roles/run.invoker

gcloud pubsub topics create cloud-builds

gcloud pubsub subscriptions create ci-results --topic=cloud-builds --push-endpoint=https://ci-results-ciaf4c5jya-ey.a.run.app --push-auth-service-account=cloud-run-pubsub-invoker@vizzu-ci.iam.gserviceaccount.com
```