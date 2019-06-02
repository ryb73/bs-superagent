open Jest;
open Expect;
open Superagent;
open PromEx;

[@bs.module] external setupNock: unit => unit = "../../../test/nock.js";
setupNock();

describe("get", () => {
    testPromise("text", () => {
        get("http://www.ok.com/get/text")
        |> end_
        |> map(({ text }) => {
            expect(text)
            |> toBe("thing")
        });
    });

    testPromise("json", () => {
        get("http://www.ok.com/get/json")
        |> setHeader(Accept(ApplicationJson))
        |> end_
        |> map(({ body }) => {
            expect(body)
            |> toBe(Some(Js.Json.string("thing")))
        });
    });
});

describe("query strings", () => {
    testPromise("query", () => {
        get("http://www.ok.com/get/query")
        |> query("hey", "bub")
        |> end_
        |> map(({ text }) => {
            expect(text)
            |> toBe("/get/query?hey=bub")
        });
    });

    testPromise("queryMultiple", () => {
        let req = [|
            ("one", "2"),
            ("three", "4"),
        |]
        |> Js.Dict.fromArray;

        get("http://www.ok.com/get/query")
        |> queryMultiple(req)
        |> end_
        |> map(({ text }) => {
            expect(text)
            |> toBe("/get/query?one=2&three=4")
        });
    });
});

describe("encoded data", () => {
    describe("send", () => {
        let expectData = (setType) => {
            let dict = [|
                ("dk", Js.Json.string("dict key")),
                ("DK", Js.Json.string("dict key is here")),
            |]
            |> Js.Dict.fromArray
            |> Js.Json.object_;

            post("http://www.ok.com/post/form")
            |> setType
            |> sendKV("kay", Js.Json.string("vee"))
            |> sendKV("arr", Js.Json.array([|Js.Json.string("r")|]))
            |> sendJson(dict)
            |> end_
            |> map(({ text }) => expect(text));
        };

        describe("form", () => {
            testPromise("type_", () => {
                expectData(type_(`form))
                |> map(toBe("kay=vee&arr%5B0%5D=r&dk=dict%20key&DK=dict%20key%20is%20here"))
            });

            testPromise("setHeader", () => {
                expectData(setHeader(ContentType(ApplicationXWwwFormUrlencoded)))
                |> map(toBe("kay=vee&arr%5B0%5D=r&dk=dict%20key&DK=dict%20key%20is%20here"))
            });
        });

        describe("json", () => {
            testPromise("type_", () => {
                expectData(type_(`json))
                |> map(toBe("{\"kay\":\"vee\",\"arr\":[\"r\"],\"dk\":\"dict key\",\"DK\":\"dict key is here\"}"))
            });

            testPromise("setHeader", () => {
                expectData(setHeader(ContentType(ApplicationJson)))
                |> map(toBe("{\"kay\":\"vee\",\"arr\":[\"r\"],\"dk\":\"dict key\",\"DK\":\"dict key is here\"}"))
            });
        });
    });
});
