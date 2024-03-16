class NotLoginError(RuntimeError):
    def __init__(self, error_msg):
        self.error_msg = error_msg


class InvalidPasswordError(RuntimeError):
    def __init__(self, error_msg):
        self.error_msg = error_msg


class UsernameAlreadyExistsError(RuntimeError):
    def __init__(self, error_msg):
        self.error_msg = error_msg


class UserDoesNotExistError(RuntimeError):
    def __init__(self, error_msg):
        self.error_msg = error_msg


class InvalidCredentialsError(RuntimeError):
    def __init__(self, error_msg):
        self.error_msg = error_msg


class IllegalOperationError(RuntimeError):
    def __init__(self, error_msg):
        self.error_msg = error_msg
