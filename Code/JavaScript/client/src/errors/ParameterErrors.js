import ParameterError from './ParameterError';
export default class ParameterErrors {
    static throwNotProvided(name) {
        throw new ParameterError(`No parameter "${name}" was provided`);
    }
    static throwNotArray(name) {
        throw new ParameterError(`The parameter "${name}" was not an array`);
    }
    static throwNotOfType(name, type) {
        throw new ParameterError(`The parameter "${name}" was not of type "${type}"`);
    }
}