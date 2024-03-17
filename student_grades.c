#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBJECTS 10
#define MAX_STUDENTS 100

// Define the structure for representing student marks
struct Marks {
    char subjects[MAX_SUBJECTS][50]; // Array to store subject names
    int marks[MAX_SUBJECTS];
    int credits[MAX_SUBJECTS];
};

// Define the structure for representing a student
struct Student {
    int id;
    char name[50];
    int age;
    struct Marks marks; // Nested structure for student marks
    float cgpa; // Cumulative Grade Point Average
};

void addStudent(struct Student students[], int numStudents, int numSubjects);
void updateStudent(struct Student students[], int numStudents, int numSubjects);
void deleteStudent(struct Student students[], int numStudents);
void displayAllStudents(struct Student students[], int numStudents);
void saveDataToCSV(struct Student students[], int numStudents, int numSubjects);
float calculateCGPA(struct Marks marks, int numSubjects);

int main() {
    int numSubjects;
    printf("Enter the number of subjects: ");
    scanf("%d", &numSubjects);
    getchar(); // Clear the newline character from the input buffer
    if (numSubjects <= 0 || numSubjects > MAX_SUBJECTS) {
        printf("Invalid number of subjects. Please enter a number between 1 and %d.\n", MAX_SUBJECTS);
        return 1;
    }

    struct Student students[MAX_STUDENTS];
    int numStudents = 0;
    int choice;

    do {
        printf("\nMenu:\n");
        printf("1. Add Student\n");
        printf("2. Update Student\n");
        printf("3. Delete Student\n");
        printf("4. Display All Students\n");
        printf("5. Save Data to CSV\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(students, numStudents, numSubjects);
                numStudents++;
                break;
            case 2:
                updateStudent(students, numStudents, numSubjects);
                break;
            case 3:
                deleteStudent(students, numStudents);
                numStudents--;
                break;
            case 4:
                displayAllStudents(students, numStudents);
                break;
            case 5:
                saveDataToCSV(students, numStudents, numSubjects);
                break;
            case 6:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

void addStudent(struct Student students[], int numStudents, int numSubjects) {
    if (numStudents < MAX_STUDENTS) {
        struct Student newStudent;

        printf("Enter student ID: ");
        scanf("%d", &newStudent.id);
        getchar(); // Clear the newline character from the input buffer

        printf("Enter student name: ");
        fgets(newStudent.name, sizeof(newStudent.name), stdin);
        newStudent.name[strcspn(newStudent.name, "\n")] = '\0'; // Remove trailing newline
        
        printf("Enter student age: ");
        scanf("%d", &newStudent.age);
        getchar(); // Clear the newline character from the input buffer

        // Input marks and credits for each subject
        printf("Enter marks and credits for each subject:\n");
        for (int i = 0; i < numSubjects; i++) {
            printf("Subject %d name: ", i+1);
            fgets(newStudent.marks.subjects[i], sizeof(newStudent.marks.subjects[i]), stdin);
            newStudent.marks.subjects[i][strcspn(newStudent.marks.subjects[i], "\n")] = '\0'; // Remove trailing newline
            
            printf("Marks for %s: ", newStudent.marks.subjects[i]);
            scanf("%d", &newStudent.marks.marks[i]);
            getchar(); // Clear the newline character from the input buffer

            printf("Credits for %s: ", newStudent.marks.subjects[i]);
            scanf("%d", &newStudent.marks.credits[i]);
            getchar(); // Clear the newline character from the input buffer
        }

        // Calculate CGPA
        newStudent.cgpa = calculateCGPA(newStudent.marks, numSubjects);

        students[numStudents] = newStudent;

        printf("Student added successfully.\n");
    } else {
        printf("Maximum number of students reached.\n");
    }
}

void updateStudent(struct Student students[], int numStudents, int numSubjects) {
    int id, found = 0;

    printf("Enter the ID of the student to update: ");
    scanf("%d", &id);

    for (int i = 0; i < numStudents; i++) {
        if (students[i].id == id) {
            printf("Enter updated information for student %d:\n", id);
            printf("Enter student name: ");
            fgets(students[i].name, sizeof(students[i].name), stdin);
            getchar();
            students[i].name[strcspn(students[i].name, "\n")] = '\0'; // Remove trailing newline
            getchar();

            printf("Enter student age: ");
            scanf("%d", &students[i].age);
            getchar();

            printf("Enter updated marks and credits for each subject:\n");
            for (int j = 0; j < numSubjects; j++) {
                printf("Marks for %s: ", students[i].marks.subjects[j]);
                scanf("%d", &students[i].marks.marks[j]);
                getchar(); 

                printf("Credits for %s: ", students[i].marks.subjects[j]);
                scanf("%d", &students[i].marks.credits[j]);
                getchar(); 
            }

            students[i].cgpa = calculateCGPA(students[i].marks, numSubjects);

            printf("Student updated successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }
}

void deleteStudent(struct Student students[], int numStudents) {
    int id, found = 0;

    printf("Enter the ID of the student to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < numStudents; i++) {
        if (students[i].id == id) {
            for (int j = i; j < numStudents - 1; j++) {
                students[j] = students[j + 1];
            }
            printf("Student with ID %d deleted successfully.\n", id);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }
}

void displayAllStudents(struct Student students[], int numStudents) {
    if (numStudents > 0) {
        printf("\nAll Students:\n");
        printf("ID\tName\tAge\tCGPA\n");
        for (int i = 0; i < numStudents; i++) {
            printf("%d\t%s\t%d\t%.2f\n", students[i].id, students[i].name, students[i].age, students[i].cgpa);
        }
    } else {
        printf("No students to display.\n");
    }
}

void saveDataToCSV(struct Student students[], int numStudents, int numSubjects) {
    FILE *file = fopen("students.csv", "a"); 

    if (file != NULL) {
       
        long pos = ftell(file);
        if (pos == 0) {
            
            fprintf(file, "ID,Name,Age,CGPA");
            for (int i = 0; i < numSubjects; i++) {
                fprintf(file, ",%s Marks,%s Credits", students[0].marks.subjects[i], students[0].marks.subjects[i]);
            }
            fprintf(file, "\n");
        }
  
        for (int i = 0; i < numStudents; i++) {
            fprintf(file, "%d,%s,%d,%.2f", students[i].id, students[i].name, students[i].age, students[i].cgpa);
            for (int j = 0; j < numSubjects; j++) {
                fprintf(file, ",%d,%d", students[i].marks.marks[j], students[i].marks.credits[j]);
            }
            fprintf(file, "\n");
        }

        fclose(file);
        printf("Student data appended to students.csv successfully.\n");
    } else {
        printf("Error opening file for writing.\n");
    }
}

float calculateCGPA(struct Marks marks, int numSubjects) {
    float totalGradePoints = 0.0;
    int totalCredits = 0;

    for (int i = 0; i < numSubjects; i++) {
        int marksInRange = marks.marks[i];
        float gradePoint = 0.0;

        if (marksInRange >= 90)
            gradePoint = 10.0;
        else if (marksInRange >= 80)
            gradePoint = 9.0;
        else if (marksInRange >= 70)
            gradePoint = 8.0;
        else if (marksInRange >= 60)
            gradePoint = 7.0;
        else if (marksInRange >= 50)
            gradePoint = 6.0;
        else if (marksInRange >= 40)
            gradePoint = 5.0;
        else if (marksInRange <40)
            gradePoint = 0;

        totalGradePoints += gradePoint * marks.credits[i];
        totalCredits += marks.credits[i];
    }

    float cgpa = 0.0;
    if (totalCredits != 0) {
        cgpa = (totalGradePoints / totalCredits) / 10.0; 
    }

    return cgpa;
}
