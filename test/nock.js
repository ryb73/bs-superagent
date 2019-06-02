const nock = require("nock");

module.exports = () => {
    const top = nock("http://www.ok.com");

    top
    .get("/get/text")
    .reply(200, "thing")

    top
    .get("/get/json")
    .matchHeader("accept", "application/json")
    .reply(200, '"thing"', {
        "Content-Type": "application/json"
    })

    top
    .get("/get/query")
    .twice()
    .query(true)
    .reply(200, (uri) => uri)

    top
    .post("/post/form")
    .times(6)
    .reply(200, (uri, body) => body)
};
