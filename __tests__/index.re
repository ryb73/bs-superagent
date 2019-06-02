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

describe("post", () => {
    describe("send", () => {
        let sendMixed = (req) => Js.Json.(
            req
            |> sendKV("kay", string("vee"))
            |> sendKV("keyedArr", array([|string("r")|]))
            |> sendArray([|string("r"),string("r")|])
            |> sendDict(
                [|
                    ("dk", string("dict key")),
                    ("DK", string("dict key is here")),
                |]
                |> Js.Dict.fromArray
            )
        );

        let expectData = (setType, sendData) => {
            post("http://www.ok.com/post/form")
            |> setType
            |> sendData
            |> end_
            |> map(({ text }) => expect(text));
        };

        describe("form", () => {
            testPromise("type_", () => {
                expectData(type_(`form), sendMixed)
                |> map(toBe("0=r&1=r&kay=vee&keyedArr%5B0%5D=r&dk=dict%20key&DK=dict%20key%20is%20here"))
            });

            testPromise("setHeader", () => {
                expectData(setHeader(ContentType(ApplicationXWwwFormUrlencoded)), sendMixed)
                |> map(toBe("0=r&1=r&kay=vee&keyedArr%5B0%5D=r&dk=dict%20key&DK=dict%20key%20is%20here"))
            });

            testPromise("raw", () =>
                expectData(type_(`form), send("rawdata!@#$%^&*()"))
                |> map(toBe("rawdata!@#$%^&*()"))
            );
        });

        describe("json", () => {
            testPromise("type_", () => {
                expectData(type_(`json), sendMixed)
                |> map(toBe({|{"0":"r","1":"r","kay":"vee","keyedArr":["r"],"dk":"dict key","DK":"dict key is here"}|}))
            });

            testPromise("setHeader", () => {
                expectData(setHeader(ContentType(ApplicationJson)), sendMixed)
                |> map(toBe({|{"0":"r","1":"r","kay":"vee","keyedArr":["r"],"dk":"dict key","DK":"dict key is here"}|}))
            });
        });
    });
});
