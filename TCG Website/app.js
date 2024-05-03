import express from 'express';
import dotenv from 'dotenv';
import { MongoClient, ServerApiVersion } from 'mongodb';
import mongoose from 'mongoose';
import { reportRouter } from './routes/reportRoutes.js'
import { authRouter } from './routes/authRoutes.js'
import fs from 'fs'
import cookieParser from 'cookie-parser'
import { checkUser } from  './middleware/authMiddleware.js'

dotenv.config();
const PORT = process.env.PORT || 3000;
const MongoDB_Pass = process.env.MONGODB_PASSWORD;
const dbURI = `mongodb+srv://tiennp03:${MongoDB_Pass}@cluster0.t192nb4.mongodb.net/RotK?retryWrites=true&w=majority&appName=Cluster0`;

const app = express();
app.use(express.json());
app.use(cookieParser());
app.use(express.urlencoded());
app.use(express.static('public'));

app.set('view engine', 'ejs');



//Routes
//Basic Routes
app.get('/', checkUser, (req, res) => {
    const jsonString = fs.readFileSync('./report_array.txt', 'utf8');
    const array = JSON.parse(jsonString);
    res.render('index', {array});
});

app.use(checkUser, reportRouter);
app.use(checkUser, authRouter);

mongoose.connect(dbURI)
    .then((result) => console.log('Successful connection to MongoDB'))
    .catch((err) => console.log(err));

app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});