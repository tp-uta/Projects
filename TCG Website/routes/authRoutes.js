import express from 'express';
import * as authController from '../controller/authController.js'

const authRouter = express.Router();

//Sign Ups
authRouter.get('/signup', authController.signup_get);
authRouter.post('/signup', authController.signup_post)


//Logins
authRouter.get('/login', authController.login_get);
authRouter.post('/login', authController.login_post);

//Log Out
authRouter.get('/logout', authController.logout_get);


export { authRouter }