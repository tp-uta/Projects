import { spawn } from 'child_process'

//Python Analysis
export async function runPythonScript(scriptPath, arg1, arg2) {
    return new Promise((resolve, reject) => {
        const pythonProcess = spawn('python', [scriptPath, arg1, arg2]);
        pythonProcess.stderr.pipe(process.stderr);
        let result = '';

        pythonProcess.stdout.on('data', data => {
            result += data.toString();
        });

        pythonProcess.on('close', code => {
            if (code === 0) {
                const parsedResult = JSON.parse(result);
                resolve(parsedResult);
            } else {
                reject(new Error(`Python process exited with code ${code}`));
            }
        });

        pythonProcess.on('error', error => {
            reject(error);
        });
    });
}

//Python Web Scrape
export async function runPythonWebScrape(scriptPath, arg1) {
    return new Promise((resolve, reject) => {
        const pythonProcess = spawn('python', [scriptPath, arg1]);
        pythonProcess.stderr.pipe(process.stderr);
        let result = '';

        pythonProcess.stdout.on('data', data => {
            result += data.toString();
        });

        pythonProcess.on('close', code => {
            if (code === 0) {
                const parsedResult = JSON.parse(result);
                resolve(parsedResult);
            } else {
                reject(new Error(`Python process exited with code ${code}`));
            }
        });

        pythonProcess.on('error', error => {
            reject(error);
        });
    });
}