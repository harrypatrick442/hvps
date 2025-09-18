import React, { Component } from 'react';
import ParameterErrors from '../errors/ParameterErrors';
const { throwNotProvided, throwNotOfType} = ParameterErrors;
export default class ReactTypeChecking{
	static isClassComponent(component) {
		return (
			typeof component === 'function' && 
			!!component.prototype.isReactComponent
		)
	}

	static isFunctionComponent(component) {
		return (
			typeof component === 'function' && 
			String(component).includes('return React.createElement')
		)
	}

	static isReactComponent(component) {
		return (
			ReactTypeChecking.isClassComponent(component) || 
			ReactTypeChecking.isFunctionComponent(component)
		)
	}

	static isElement(element) {
		return React.isValidElement(element);
	}

	static isDOMTypeElement(element) {
		return ReactTypeChecking.isElement(element) && typeof element.type === 'string';
	}

	static isCompositeTypeElement(element) {
		return ReactTypeChecking.isElement(element) && typeof element.type === 'function';
	}
}