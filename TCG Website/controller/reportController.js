import { OCG } from '../models/ocg.js';
import { runPythonScript , runPythonWebScrape } from '../pyfunc.js';
import fs from 'fs'
//

export const report_pages = (req, res) => {
    const id = req.params.id;
    res.render(`Report${id}`, {id});
}

export const report_create_page = (req, res) => {
    res.render('create');
}

export const report_get = async(req, res) => {
    try{
        const new_id = req.params.id;
        const jsonString = fs.readFileSync('./report_array.txt', 'utf8');
        const storedArray = JSON.parse(jsonString);
        let old_id;
        for(let i in storedArray) {
            if(storedArray[i] === new_id) {
                old_id = storedArray[i-1];
                break;
            }
        }

        const query1 = await OCG.findOne({ report_num: old_id }).exec();
        const query2 = await OCG.findOne({ report_num: new_id }).exec();

        const report = await runPythonScript('./py_analysis.py', JSON.stringify(query1['data']), JSON.stringify(query2['data']));

        res.json(report);
    } catch (error) {
        console.log(error);
        res.status(500).json({ error:error.message });
    }
}

export const report_create_post = async(req, res) => {
    const temp = await runPythonWebScrape('./py_webscrape.py', req.body['URL']);
    const check = await OCG.exists( {title: temp['title']} );
    if (!check) {
        //Create OCG Model
        const ocg = new OCG({
            title: temp['title'],
            data: temp['data'],
            report_num: temp['report_num']
        });

        //Get sorted array file and append new ocg
        let jsonString = fs.readFileSync('./report_array.txt', 'utf8');
        let storedArray = JSON.parse(jsonString);
        storedArray.push(ocg['report_num']);
        storedArray.sort((a, b) => a - b);

        jsonString = JSON.stringify(storedArray);
        fs.writeFileSync('./report_array.txt', jsonString);

        //Save ocg to collection
        ocg.save()
            .then((result) => {
                res.send(result);
            })
            .catch((err) => {
                console.log(err);
                res.redirect('/report/create');
            })

    } else {
        console.log('Data already exists in the database');
        res.redirect('/report/create');
    }
}