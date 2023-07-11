// import { loader } from "./loader";
// import {get, set } from "idb-keyval";
// import "es6-promise/auto";
// import {get, set } from "idb-keyval/dist/index.js";
// import {get, set } from "idb-keyval/dist/compat.js";
export const dbApp = {};
let indexedDB = window.indexedDB || window.mozIndexedDB || window.webkitIndexedDB || window.msIndexedDB || self.indexedDB,
	IDBTransaction = window.IDBTransaction || window.webkitIDBTransaction || window.msIDBTransaction || self.IDBTransaction;
var db;
var dbPubName = "spinPlateImgs";
let dbWRK;
let dbPromise;
////////////////////////////////////////
import { openDB } from "idb/with-async-ittr";
// import loadWorker from "./workers/loadWorker?worker&inline";
// import dbWorker from "./workers/dbWorker?worker&inline";

dbApp.init = async dbStorage => {
	console.log("DB-init", dbStorage);
	//check for support
	if (!("indexedDB" in window)) {
		console.log("This browser doesn't support IndexedDB");
		return;
	}

	await db(dbStorage);
};
db = async dbStorage => {
	return await openDB(dbStorage, 1, {
		upgrade(db) {
			const store = db.createObjectStore(dbStorage, {
				keyPath: "title",
				autoIncrement: true,
			});
			store.createIndex("title", "title", {
				unique: true,
			});
            store.createIndex("b64", "b64", {
				unique: true,
			});
			store.createIndex("dateModified", "dateModified", {
				unique: false,
			});
		},
	});
};
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

dbApp.addPublication = async (title, dbName, blob,b64) => {
	console.log(title, dbName, blob);
	let db = await openDB(dbName, 1);
	let x = await db.get(dbName, title);
	if (!x) {
		await db.add(dbName, {
			title: title,
			blob: blob,
            b64:b64,
			dateModified: new Date().getTime(),
		});
		console.log("added");
		return "added";
	} else {
		// console.log("title: " + title + " ---- DB: " + dbName + " Already exists");

		await db.put(dbName, {
			title: title,
			blob: blob,
            b64:b64,
			dateModified: new Date().getTime(),
		});
		console.log("updated");
		return "updated";
	}
	db.close();
};

dbApp.loadPublication = async (objName, dbName) => {
	// console.log("objName", objName);
	// return new Promise((resolve, reject) => {
	const db = await openDB(dbName, 1);
	let x = await db.get(dbName, objName);
	return x;
};
dbApp.checkPublication = async (objName, database) => {
	const db1 = await openDB(database, 1);
	// const req = await db1.getAll(database);
	// const keys = await db1.getAllKeys(database);
	let transaction = db1.transaction(database, "readonly");
	let result;
	try {
		result = await transaction.objectStore(database).get(objName);
	} catch {}
	db1.close();
	// console.log('checkPublication - ', result)
	return result;
};

dbApp.loadImage = async objName => {
	const recValue = await loader(`${objName}`);
	console.log("recValue ", recValue);
	let blbJpg = recValue;
	console.log(blbJpg);
	// let blbBmp = await createImageBitmap(blbJpg);
	// make PNG
	// let blbPNG = await dbApp.bmpTOblob(blbBmp);
	// let b64 = await dbApp.convToB64(blbPNG);
	let b64 = await dbApp.convToB64(blbJpg);
	// console.log('   - - - ', blb2);
	// let blb = await fetch(recValue).then(res => res.blob())
	let obj = {
		title: objName,
		// blob: blbPNG,
		blob: blbJpg,
		b64: b64,
		dateModified: new Date().getTime(),
	};

	// console.log("Loaded Combined OBJ : " + obj);
	return obj;
};

dbApp.readAllDB = async dbName => {
	const db1 = await openDB(dbName, 1);
	const req = await db1.getAll(dbName);
	const keys = await db1.getAllKeys(dbName);
	db1.close();
	return req;
};

dbApp.deletePublication = async (dbName, fileName) => {
	const db2 = await openDB(dbName, 1);
	let transaction = db2.transaction(dbName, "readwrite");
	try {
		let superCat = await transaction.objectStore(dbName).get(fileName);
		transaction.objectStore(dbName).delete(fileName);
	} catch (e) {}
	db2.close();
};
dbApp.deleteDatabase = dbName => {
	var req = indexedDB.open(dbName, 1);
	var DBRequest = indexedDB.deleteDatabase(dbName);
	DBRequest.onsuccess = function (event) {
		console.log("Database " + dbName + " deleted");
	};
	console.warn(DBRequest);
};
// dbApp.deleteDatabase("dbdb");
// dbApp.deleteDatabase("spinPlateImgs");
// dbApp.deleteDatabase("BRATAN-dbdb");

dbApp.deleteObjStore = objStoreBGName => {
	// nowPlayingCustom1Artwork = nowPlayingArtwork = "";

	var req;
	var store = dbApp.getObjectStore(objStoreBGName, "readwrite");
	try {
		req = store.clear();
	} catch (e) {}
};
dbApp.bmpTOblob = bmp => {
	// const ctx = document.createElement("canvas").getContext("2d");
	return new Promise(res => {
		const cnv = document.createElement("canvas");
		cnv.width = bmp.width;
		cnv.height = bmp.height;
		cnv.getContext("bitmaprenderer").transferFromImageBitmap(bmp);
		cnv.toBlob(blb => res(blb));
	});
};
dbApp.convToB64 = function (blob) {
	return new Promise(callback => {
		let reader = new FileReader();
		reader.onload = function () {
			callback(this.result);
		};
		reader.readAsDataURL(blob);
	});
};
let dlWorker;
async function initDlWorker() {
	return new Promise((resolve, reject) => {
		dlWorker = new loadWorker();

		dlWorker.postMessage({
			msg: "init",
			data: "101010101FROM-DB-SCRIPT",
		});

		function handler(e) {
			switch (e.data.msg) {
				case "initFinished":
					dlWorker.removeEventListener("message", handler);
					resolve();
					break;
				default:
					break;
			}
		}
		dlWorker.addEventListener("message", handler);
	});
}

// function loader(src) {
//     return new Promise((resolve, reject) => {
//         if (!dlWorker) {
//             initDlWorker().then(get);
//         } else {
//             get();
//         }
//         // function handler(e) {
//         function get() {
//             dlWorker.postMessage({
//                 msg: "getIMG",
//                 src
//             });
//             dlWorker.addEventListener("message", e => {
//                 dlWorker.terminate();
//                 dlWorker = false;
//                 // resolve(e.data.bmp);
//                 resolve(e.data);
//             });
//         }
//     });
// }
async function loader(src) {
	console.log("function -loader - db " + src);
	return new Promise((resolve, reject) => {
		let blb;
		try {
			blb = fetch(src, {
				mode: "cors",
			}).then(response => response.blob());
			// .then(blb => createImageBitmap(blb));

			// .then(result => );
			// console.log(blb);
			resolve(blb);
		} catch (e) {
			console.log(e);
		} finally {
			console.log("We do cleanup here");
		}
	});
}
//////////////
// }).catch(e => {
// 	console.log("Loader Error  - - " + e);
// });
// }
function promiseReq(req) {
	return new Promise((resolve, reject) => {
		req.onsuccess = () => resolve(req.result);
		req.onerror = () => reject(req.error);
	});
}
