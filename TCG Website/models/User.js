import mongoose from 'mongoose'
import isEmail from 'validator/lib/isEmail.js';
import bcrypt from 'bcrypt'


const userSchema = new mongoose.Schema({
    email: {
        type: String,
        required: [true, 'Email is required'],
        unique: true,
        lowercase: true,
        validate: [isEmail, 'Invalid email']
    },
    password: {
        type: String,
        required: [true, 'Password is required'],
        minlength: [6, 'Password must be at least 6 characters']
    },
});

//Function for after model is saved to database
userSchema.post('save', function(doc, next) {
    console.log('new user was created', doc);
    next();
})

//Function for before model is saved to database
userSchema.pre('save', async function(next) {
    const salt = await bcrypt.genSalt();
    this.password = await bcrypt.hash(this.password, salt);
    next();
})

//static method to log in user
userSchema.statics.login = async function(email, password) {
    const user = await this.findOne({email});
    if (user) {
        const auth = bcrypt.compare(password, user.password);
        if (auth) {
            return user;
        }
        throw Error('Incorrect password');
    }
    throw Error("Email doesn't exist");
}

const User = mongoose.model('user', userSchema);

export {User};